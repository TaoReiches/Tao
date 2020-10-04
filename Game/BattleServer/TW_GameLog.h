#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "spdlog/spdlog.h"

#define TW_LOG(msg) spdlog::info(msg)
#define TW_WARN(msg) spdlog::warn(msg)
#define TW_ERROR(msg) spdlog::critical(msg)
