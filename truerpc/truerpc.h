#pragma once

#include "Router.h"

#include <stdexcept>
#include <list>
#include <queue>
#include <map>
#include <stack>
#include <boost/utility.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

//==================================================================================================
class RPC : public IRouter, boost::noncopyable
{
public :
    RPC(IStub *stub);
    ~RPC();

    void message(Data const &request);
    void signal(Data const &request, Data &response);
    void beginSlot(Data const &response);
    void endSlot(Data const &request);
    void start(IChannel *channel);
    void stop();
    bool running();

private :
    void setRunning(bool running);
    void *addSync();
    void waitSync(void *sync);
    void signalSync(void *sync);
    void removeSync(void *sync);
    void putResponse(Data const *response);
    Data const *getResponse(void * const sync);
    void reader();

private :
    typedef std::list<boost::condition_variable *> SyncList;
    typedef std::list<Data const *> ResponseList;

    bool _running;
    boost::mutex _runningMutex;
    boost::mutex _startingMutex;
    boost::thread _reader;
    SyncList _syncList;
    boost::mutex _syncMutex;
    ResponseList _responseList;
    boost::mutex _responseListMutex;
    std::map<boost::thread::id, std::stack<void *> > _stack;
    boost::mutex _stackMutex;
};
//==================================================================================================
struct ERPC : std::runtime_error
{
explicit
    ERPC(std::string const &message)
    : runtime_error(message)
    {}
};
//==================================================================================================
struct ERPCInit : ERPC
{
explicit
    ERPCInit(std::string const &message)
    : ERPC(message)
    {}
};
//==================================================================================================
struct ERPCSync : ERPC
{
explicit
    ERPCSync(std::string const &message)
    : ERPC(message)
    {}
};
//==================================================================================================
