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

#include "QUICTypes.h"
#include "ts/Map.h"
#include <cstddef>

class QUICTransportParameterId
{
public:
  enum {
    INITIAL_MAX_STREAM_DATA = 0,
    INITIAL_MAX_DATA,
    INITIAL_MAX_STREAM_ID,
    IDLE_TIMEOUT,
    TRUNCATE_CONNECTION_ID,
    MAX_PACKET_SIZE,
  };

  explicit operator bool() const { return true; }

  bool
  operator==(const QUICTransportParameterId &x) const
  {
    return this->_id == x._id;
  }

  bool
  operator==(uint16_t &x) const
  {
    return this->_id == x;
  }

  operator uint16_t() const { return _id; };

  QUICTransportParameterId() : _id(0){};
  QUICTransportParameterId(uint16_t id) : _id(id){};

private:
  uint16_t _id = 0;
};

typedef struct _QUICTransportParameterValue {
  _QUICTransportParameterValue(){};
  _QUICTransportParameterValue(const uint8_t *str) : data(str), len(str ? strlen(reinterpret_cast<const char *>(str)) : 0){};
  _QUICTransportParameterValue(const uint8_t *_data, uint16_t _len) : data(_data), len(_len){};
  const uint8_t *data = nullptr;
  uint16_t len        = 0;
} QUICTransportParameterValue;

class QUICTransportParameters
{
public:
  QUICTransportParameters(const uint8_t *buf) : _buf(buf){};
  QUICTransportParameterValue get(QUICTransportParameterId id) const;
  void add(QUICTransportParameterId id, QUICTransportParameterValue value);
  void store(uint8_t *buf, uint16_t *len) const;

protected:
  virtual std::ptrdiff_t _parameters_offset() const = 0;
  virtual void _store(uint8_t *buf, uint16_t *len) const = 0;
  const uint8_t *_buf;
  Map<QUICTransportParameterId, QUICTransportParameterValue> _parameters;
};

class QUICTransportParametersInClientHello : public QUICTransportParameters
{
public:
  QUICTransportParametersInClientHello(QUICVersion negotiated_version, QUICVersion initial_version)
    : QUICTransportParameters(nullptr), _negotiated_version(negotiated_version), _initial_version(initial_version){};
  QUICTransportParametersInClientHello(const uint8_t *buf) : QUICTransportParameters(buf){};
  QUICVersion negotiated_version() const;
  QUICVersion initial_version() const;

protected:
  std::ptrdiff_t _parameters_offset() const override;
  void _store(uint8_t *buf, uint16_t *len) const override;

private:
  QUICVersion _negotiated_version = 0;
  QUICVersion _initial_version    = 0;
};

class QUICTransportParametersInEncryptedExtensions : public QUICTransportParameters
{
public:
  QUICTransportParametersInEncryptedExtensions() : QUICTransportParameters(nullptr){};
  QUICTransportParametersInEncryptedExtensions(const uint8_t *buf) : QUICTransportParameters(buf){};
  const uint8_t *supported_versions(uint16_t *n) const;
  void add_version(QUICVersion version);

protected:
  std::ptrdiff_t _parameters_offset() const override;
  void _store(uint8_t *buf, uint16_t *len) const override;

  uint8_t _n_versions        = 0;
  QUICVersion _versions[256] = {};
};
