// -*- C++ -*-
/*!
 * @file  AvsDeviceSDKWrapper.cpp
 * @brief avsdevicesdkwrapper
 * @date $Date$
 *
 * $Id$
 */

#include "AvsDeviceSDKWrapper.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pid;

int pipefd[2];

// Module specification
// <rtc-template block="module_spec">
static const char* avsdevicesdkwrapper_spec[] =
  {
    "implementation_id", "AvsDeviceSDKWrapper",
    "type_name",         "AvsDeviceSDKWrapper",
    "description",       "avsdevicesdkwrapper",
    "version",           "1.0.0",
    "vendor",            "hiroyasutsuji",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.execution_path", "/execution/path",

    // Widget
    "conf.__widget__.execution_path", "text",
    // Constraints

    "conf.__type__.execution_path", "string",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
AvsDeviceSDKWrapper::AvsDeviceSDKWrapper(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_orderIn("orderin", m_order)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
AvsDeviceSDKWrapper::~AvsDeviceSDKWrapper()
{
}



RTC::ReturnCode_t AvsDeviceSDKWrapper::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("orderin", m_orderIn);
  
  // Set OutPort buffer
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("execution_path", m_path, "/execution/path");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t AvsDeviceSDKWrapper::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t AvsDeviceSDKWrapper::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t AvsDeviceSDKWrapper::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t AvsDeviceSDKWrapper::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t AvsDeviceSDKWrapper::onDeactivated(RTC::UniqueId ec_id)
{ 
  close(pipefd[0]);
  char *s = "q";
  write(pipefd[1], s, strlen(s));
  close(pipefd[1]);
  return RTC::RTC_OK;
}


RTC::ReturnCode_t AvsDeviceSDKWrapper::onExecute(RTC::UniqueId ec_id)
{
  if (m_orderIn.isNew())
    {
      m_orderIn.read();
      if (!strcmp(m_order.data, "start"))
        {
	if (pipe(pipefd) < 0) {
		perror("pipe");
		return RTC::RTC_OK;
	}
	pid_t pid = fork();
	if (pid < 0) {
		perror("fork");
		return RTC::RTC_OK;
	} else if (pid == 0) {
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
                execl(m_path.c_str(), NULL);
		return RTC::RTC_OK;
	}
        }
      if (!strcmp(m_order.data, "withoutalexa"))
        {
         close(pipefd[0]);
         char *s = "t";
	 write(pipefd[1], s, strlen(s));
         close(pipefd[1]);
         return RTC::RTC_OK;
        }
      if (!strcmp(m_order.data, "reset"))
        {
         close(pipefd[0]);
         char *s = "q";
         write(pipefd[1], s, strlen(s));
         close(pipefd[1]);
         return RTC::RTC_OK;
         if (pipe(pipefd) < 0) 
           {
	    perror("pipe");
	    return RTC::RTC_OK;
	   }

	pid_t pid = fork();
	if (pid < 0) {
		perror("fork");
		return RTC::RTC_OK;
	} else if (pid == 0)
                 {
		  close(pipefd[1]);
		  dup2(pipefd[0], STDIN_FILENO);
		  close(pipefd[0]);
                  execl(m_path.c_str(), NULL);
		  return RTC::RTC_OK;
	         }
        }
      if (!strcmp(m_order.data, "stop"))
        {
         close(pipefd[0]);
         char *s = "q";
	 write(pipefd[1], s, strlen(s));
         close(pipefd[1]);
         return RTC::RTC_OK;
        }
    }
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t AvsDeviceSDKWrapper::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t AvsDeviceSDKWrapper::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t AvsDeviceSDKWrapper::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t AvsDeviceSDKWrapper::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t AvsDeviceSDKWrapper::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void AvsDeviceSDKWrapperInit(RTC::Manager* manager)
  {
    coil::Properties profile(avsdevicesdkwrapper_spec);
    manager->registerFactory(profile,
                             RTC::Create<AvsDeviceSDKWrapper>,
                             RTC::Delete<AvsDeviceSDKWrapper>);
  }
  
};


