#include "SessionAPIInterface.h"
#include <QtDebug>

SessionAPIInterface::SessionAPIInterface(int appid)
{
    qDebug() << "SessionAPIInterface::SessionAPIInterface begin";
    authapi = new AuthAPI(appid);
    connect(authapi, SIGNAL(SessionOk()), this, SLOT(OnAPISessionOk()));
    connect(authapi, SIGNAL(LoginErr(AuthAPI::ERRCODE)), this, SLOT(OnAPILoginErr(AuthAPI::ERRCODE)));
    connect(authapi, SIGNAL(SessionExpired()), this, SLOT(OnAPISessionExpired()));
    connect(authapi, SIGNAL(UserNameNeeded()), this, SLOT(OnAPIUserNameNeeded()));
    authapi->start();
}

void SessionAPIInterface::OnAPISessionOk()
{
    qDebug() << "SessionAPIInterface:OnAPISessionOk: begin";
    Event myevent(abbsd::sessionok);
    myevent.add_source(Event::ES_INTERNAL);
    emit new_event(myevent);
}

void SessionAPIInterface::OnAPILoginErr(AuthAPI::ERRCODE code)
{
    qDebug() << "SessionAPIInterface:OnAPILoginErr: begin";
    Event myevent;
    switch(code){
        case AuthAPI::BAD_USERNAME:
        case AuthAPI::ALREADY_LOGGED:
            break;
        case AuthAPI::BAD_CREDENTIALS:
            myevent = Event(abbsd::sessionbadcredentials);
            myevent.add_source(Event::ES_INTERNAL);
            emit new_event(myevent);
            break;
        case AuthAPI::SERVER_TIMEOUT:
            myevent = Event(abbsd::sessionservererror);
            myevent.add_source(Event::ES_INTERNAL);
            emit new_event(myevent);
            break;
    }
}
void SessionAPIInterface::OnAPISessionExpired()
{
    qDebug() << "SessionAPIInterface:OnAPISessionExpired: begin";
}
void SessionAPIInterface::OnAPIUserNameNeeded()
{
    qDebug() << "SessionAPIInterface:OnAPIUserNameNeeded: begin";
    Event myevent(abbsd::sessionbadusername);
    myevent.add_source(Event::ES_INTERNAL);
    emit new_event(myevent);
}

AuthAPI::APISTATUSCODE SessionAPIInterface::currentStatus()
{
    return authapi->currentStatus();
}
