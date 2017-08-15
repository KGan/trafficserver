/** @file
 *
 *  A brief file description
 *
 *  @section license License
 *
 *  Licensed to the Apache Software Foundation (ASF) under one
 *  or more contributor license agreements.  See the NOTICE file
 *  distributed with this work for additional information
 *  regarding copyright ownership.  The ASF licenses this file
 *  to you under the Apache License, Version 2.0 (the
 *  "License"); you may not use this file except in compliance
 *  with the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#pragma once

#include "QUICFrame.h"

class QUICConnection;
class QUICStreamManager;
class QUICFlowController;
class QUICCongestionController;
class QUICLossDetector;

class QUICFrameDispatcher
{
public:
  QUICFrameDispatcher(QUICConnection *connection, const std::shared_ptr<QUICStreamManager> smgr,
                      const std::shared_ptr<QUICFlowController> fctlr, const std::shared_ptr<QUICCongestionController> cctlr,
                      const std::shared_ptr<QUICLossDetector> ld);
  /*
   * Returns true if ACK frame should be sent
   */
  bool receive_frames(const uint8_t *payload, uint16_t size);

  std::shared_ptr<QUICStreamManager> streamManager               = nullptr;
  std::shared_ptr<QUICFlowController> flowController             = nullptr;
  std::shared_ptr<QUICCongestionController> congestionController = nullptr;
  std::shared_ptr<QUICLossDetector> lossDetector                 = nullptr;

private:
  QUICConnection *_connection = nullptr;
  QUICFrameFactory _frame_factory;
};