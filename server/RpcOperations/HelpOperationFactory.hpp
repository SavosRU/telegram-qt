/*
   Copyright (C) 2018 Alexander Akulich <akulichalexander@gmail.com>

   This file is a part of TelegramQt library.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

 */

#ifndef HELP_OPERATION_FACTORY_HPP
#define HELP_OPERATION_FACTORY_HPP

#include "RpcOperationFactory.hpp"
#include "ServerRpcOperation.hpp"

#include <QObject>

namespace Telegram {

namespace Server {

class HelpRpcOperation : public RpcOperation
{
    Q_OBJECT
    Q_DISABLE_COPY(HelpRpcOperation)
public:
    explicit HelpRpcOperation(RpcLayer *rpcLayer) : RpcOperation(rpcLayer) { }

    // Generated process methods
    bool processGetAppChangelog(RpcProcessingContext &context);
    bool processGetAppUpdate(RpcProcessingContext &context);
    bool processGetCdnConfig(RpcProcessingContext &context);
    bool processGetConfig(RpcProcessingContext &context);
    bool processGetInviteText(RpcProcessingContext &context);
    bool processGetNearestDc(RpcProcessingContext &context);
    bool processGetRecentMeUrls(RpcProcessingContext &context);
    bool processGetSupport(RpcProcessingContext &context);
    bool processGetTermsOfService(RpcProcessingContext &context);
    bool processSaveAppLog(RpcProcessingContext &context);
    bool processSetBotUpdatesStatus(RpcProcessingContext &context);
    // End of generated process methods

    // Generated run methods
    void runGetAppChangelog();
    void runGetAppUpdate();
    void runGetCdnConfig();
    void runGetConfig();
    void runGetInviteText();
    void runGetNearestDc();
    void runGetRecentMeUrls();
    void runGetSupport();
    void runGetTermsOfService();
    void runSaveAppLog();
    void runSetBotUpdatesStatus();
    // End of generated run methods

    using RunMethod = void (HelpRpcOperation::*)();
    using ProcessingMethod = bool (HelpRpcOperation::*)(RpcProcessingContext &);
    static ProcessingMethod getMethodForRpcFunction(TLValue function);

protected:
    void startImplementation() override { callMember<>(this, m_runMethod); }

    void setRunMethod(RunMethod method);

    RunMethod m_runMethod = nullptr;

    // Generated RPC members
    TLFunctions::TLHelpGetAppChangelog m_getAppChangelog;
    TLFunctions::TLHelpGetAppUpdate m_getAppUpdate;
    TLFunctions::TLHelpGetCdnConfig m_getCdnConfig;
    TLFunctions::TLHelpGetConfig m_getConfig;
    TLFunctions::TLHelpGetInviteText m_getInviteText;
    TLFunctions::TLHelpGetNearestDc m_getNearestDc;
    TLFunctions::TLHelpGetRecentMeUrls m_getRecentMeUrls;
    TLFunctions::TLHelpGetSupport m_getSupport;
    TLFunctions::TLHelpGetTermsOfService m_getTermsOfService;
    TLFunctions::TLHelpSaveAppLog m_saveAppLog;
    TLFunctions::TLHelpSetBotUpdatesStatus m_setBotUpdatesStatus;
    // End of generated RPC members
};

class HelpOperationFactory : public RpcOperationFactory
{
public:
    RpcOperation *processRpcCall(RpcLayer *layer, RpcProcessingContext &context) override;
};

} // Server namespace

} // Telegram namespace

#endif // HELP_OPERATION_FACTORY_HPP
