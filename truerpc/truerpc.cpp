#include "TrueRPC.h"

//==================================================================================================
RPC::RPC(IStub *stub)
: IRouter(stub)
{
    if (!stub)
        throw ERPCInit("No stub!");
}
//--------------------------------------------------------------------------------------------------
RPC::~RPC()
{
    stop();
}
//--------------------------------------------------------------------------------------------------
void RPC::message(Data const &request)
{
    _channel->write(request);
}
//--------------------------------------------------------------------------------------------------
void RPC::signal(Data const &request, Data &response)
{
    void *sync = addSync();

    reinterpret_cast<Sync const *>(request.data())->sender = sync;

    if (_stack[boost::this_thread::get_id()].size())
        reinterpret_cast<Sync const *>(request.data())->receiver = _stack[boost::this_thread::get_id()].top();

    _channel->write(request);

    for (;;)
    {
        waitSync(sync);

        std::auto_ptr<Data const> answer;

        answer.reset(getResponse(sync));

        if (reinterpret_cast<Sync const *>(request.data())->sender)
            _stub->onSignal(*answer);
        else
        {
            response = *answer;

            break;
        }
    }

    removeSync(sync);
}
//--------------------------------------------------------------------------------------------------
void RPC::beginSlot(Data const &response)
{
    boost::unique_lock<boost::mutex> l(_stackMutex);

    _stack[boost::this_thread::get_id()].push(reinterpret_cast<Sync const *>(response.data())->sender);
}
//--------------------------------------------------------------------------------------------------
void RPC::endSlot(Data const &request)
{
    boost::unique_lock<boost::mutex> l(_stackMutex);

    if (_stack[boost::this_thread::get_id()].size())
    {
        reinterpret_cast<Sync const *>(request.data())->receiver = _stack[boost::this_thread::get_id()].top();
        _stack[boost::this_thread::get_id()].pop();
    }

    _channel->write(request);
}
//--------------------------------------------------------------------------------------------------
void RPC::start(IChannel *channel)
{
    boost::unique_lock<boost::mutex> l(_startingMutex);

    if (running())
        return;

    _channel = channel;
    setRunning(true);
    _reader = boost::thread(&RPC::reader, this);
}
//--------------------------------------------------------------------------------------------------
void RPC::stop()
{
    boost::unique_lock<boost::mutex> l(_startingMutex);

    if (!running())
        return;

    setRunning(false);
    _reader.join();
}
//--------------------------------------------------------------------------------------------------
bool RPC::running()
{
    boost::unique_lock<boost::mutex> l(_runningMutex);

    return _running;
}
//--------------------------------------------------------------------------------------------------
void RPC::setRunning(bool running)
{
    boost::unique_lock<boost::mutex> l(_runningMutex);

    _running = running;
}
//--------------------------------------------------------------------------------------------------
void *RPC::addSync()
{
    boost::unique_lock<boost::mutex> l(_syncMutex);
    boost::condition_variable *sync = new boost::condition_variable;

    _syncList.push_back(sync);

    return sync;
}
//--------------------------------------------------------------------------------------------------
void RPC::waitSync(void *sync)
{
    boost::unique_lock<boost::mutex> l(_syncMutex);

    SyncList::iterator i = std::find(_syncList.begin(), _syncList.end(), sync);

    if (_syncList.end() == i)
        throw ERPCSync("No such synchronization!");

    boost::condition_variable *cond = reinterpret_cast<boost::condition_variable *>(*i);
    boost::system_time const timeout = boost::get_system_time() + boost::posix_time::milliseconds(50);

    while (!cond->timed_wait(l, timeout))
    {
        if (!running())
            throw ERPCSync("Client stopped!");
    }
}
//--------------------------------------------------------------------------------------------------
void RPC::signalSync(void *sync)
{
    boost::unique_lock<boost::mutex> l(_syncMutex);

    SyncList::iterator i = std::find(_syncList.begin(), _syncList.end(), sync);

    if (_syncList.end() == i)
        throw ERPCSync("No such synchronization!");

    boost::condition_variable *cond = reinterpret_cast<boost::condition_variable *>(*i);

    cond->notify_one();
}
//--------------------------------------------------------------------------------------------------
void RPC::removeSync(void *sync)
{
    boost::unique_lock<boost::mutex> l(_syncMutex);

    SyncList::iterator i = std::find(_syncList.begin(), _syncList.end(), sync);

    if (_syncList.end() == i)
        throw ERPCSync("No such synchronization!");

    _syncList.erase(i);
}
//--------------------------------------------------------------------------------------------------
void RPC::putResponse(Data const *response)
{
    boost::unique_lock<boost::mutex> l(_responseListMutex);

    _responseList.push_back(response);
}
//--------------------------------------------------------------------------------------------------
Data const *RPC::getResponse(void * const sync)
{
    boost::unique_lock<boost::mutex> l(_responseListMutex);

    Data const *ret = 0;

    ResponseList::const_iterator end = _responseList.end();

    for (ResponseList::iterator i = _responseList.begin(); end != i; ++i)
    {
        Sync const *s = reinterpret_cast<Sync const *>((*i)->data());

        if (sync == s->receiver)
        {
            ret = *i;
            _responseList.erase(i);

            break;
        }
    }

    return ret;
}
//--------------------------------------------------------------------------------------------------
void RPC::reader()
{
    while (running())
    try
    {
        std::auto_ptr<Data> response;

        for (;;)
        try
        {
            _channel->read(response, 50);
        }
        catch (EChannelReadTimeout)
        {
            if (running())
                continue;
            else
                throw int();  //to leave the nested loop
        }

        Sync *sync = reinterpret_cast<Sync *>(response->data());

        if (sync->receiver)
        {
            putResponse(response.release());
            signalSync(sync->receiver);
        }
        else
            if (sync->sender)
                _stub->onSignal(*response);
            else
                _stub->onMessage(*response);
    }
    catch (int)
    {}
    catch (std::exception)
    {
        stop();
    }
}
//==================================================================================================
