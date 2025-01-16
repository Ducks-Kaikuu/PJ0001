// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#if COMMONUSER_OSSV1
// Online Subsystem (OSS v1) includes and forward declares
//#include "OnlineSubsystemTypes.h"
using FOnlineErrorType = FOnlineError;
using ELoginStatusType = ELoginStatus::Type;
#else
#include "Online/Auth.h"
//#include "Online/Connectivity.h"
//#include "Online/OnlineError.h"
using FOnlineErrorType = UE::Online::FOnlineError;
using ELoginStatusType = UE::Online::ELoginStatus;
#endif
