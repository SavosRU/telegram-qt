#include "RemoteClientConnection.hpp"
#include "RemoteClientConnectionHelper.hpp"

#include "ApiUtils.hpp"
#include "TLTypes.hpp"
#include "CTelegramTransport.hpp"
#include "ServerApi.hpp"
#include "ServerDhLayer.hpp"
#include "ServerRpcLayer.hpp"
#include "Session.hpp"
#include "TelegramServerUser.hpp"

#include <QDateTime>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(loggingCategoryRemoteClientConnection, "telegram.server.remoteclient.connection", QtInfoMsg)

namespace Telegram {

namespace Server {

RemoteClientConnection::RemoteClientConnection(QObject *parent) :
    BaseConnection(parent)
{
    m_sendHelper = new MTProtoSendHelper(this);
    m_dhLayer = new DhLayer(this);
    m_dhLayer->setSendPackageHelper(m_sendHelper);
    m_rpcLayer = new RpcLayer(this);
    m_rpcLayer->setSendPackageHelper(m_sendHelper);
}

RpcLayer *RemoteClientConnection::rpcLayer() const
{
    return reinterpret_cast<RpcLayer*>(m_rpcLayer);
}

BaseDhLayer *RemoteClientConnection::dhLayer() const
{
    return m_dhLayer;
}

void RemoteClientConnection::setRpcFactories(const QVector<RpcOperationFactory *> &rpcFactories)
{
    rpcLayer()->setRpcFactories(rpcFactories);
}

ServerApi *RemoteClientConnection::api() const
{
    return rpcLayer()->api();
}

void RemoteClientConnection::setServerApi(ServerApi *api)
{
    rpcLayer()->setServerApi(api);
}

Session *RemoteClientConnection::session() const
{
    return rpcLayer()->session();
}

void RemoteClientConnection::setSession(Session *session)
{
    session->setConnection(this);
    rpcLayer()->setSession(session);
}

void RemoteClientConnection::sendKeyError()
{
    static const QByteArray errorPackage = QByteArray::fromHex(QByteArrayLiteral("6cfeffff"));
    m_transport->sendPacket(errorPackage);
}

bool RemoteClientConnection::processAuthKey(quint64 authKeyId)
{
    if (authKeyId == m_sendHelper->authId()) {
        return true;
    }
    if (m_sendHelper->authId()) {
        qCInfo(loggingCategoryRemoteClientConnection) << this << transport()->remoteAddress()
                                                      << "new session authKeyId"
                                                      << showbase << hex << authKeyId
                                                      << "is different from the expected"
                                                      << m_sendHelper->authId();
    } else {
        const QByteArray authKey = api()->getAuthKeyById(authKeyId);
        if (authKey.isEmpty()) {
            qCInfo(loggingCategoryRemoteClientConnection) << this << transport()->remoteAddress()
                                                          << "Unable to find an authorization with id"
                                                          << showbase << hex << authKeyId;
        } else {
            qCInfo(loggingCategoryRemoteClientConnection) << this << transport()->remoteAddress()
                                                          << "assign exists session (by auth key id)"
                                                          << showbase << hex << authKeyId;
            m_sendHelper->setAuthKey(authKey);
            return true;
        }
    }

    disconnect(m_transport, &BaseTransport::packetReceived, this, &RemoteClientConnection::onTransportPacketReceived);
    connect(m_transport, &BaseTransport::packetReceived, this, &RemoteClientConnection::sendKeyError);
    setStatus(Status::Failed, StatusReason::Local);
    sendKeyError();
    return false;
}

} // Server namespace

} // Telegram namespace
