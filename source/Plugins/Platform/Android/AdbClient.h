//===-- AdbClient.h ---------------------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef liblldb_AdbClient_h_
#define liblldb_AdbClient_h_

// C Includes

// C++ Includes

#include <list>
#include <string>

// Other libraries and framework includes
// Project includes

#include "lldb/Core/Error.h"

namespace lldb_private {

class AdbClient
{
public:
    using DeviceIDList = std::list<std::string>;

    AdbClient () = default;
    explicit AdbClient (const std::string &device_id);

    void
    SetDeviceID (const std::string& device_id);

    Error
    GetDevices (DeviceIDList &device_list);

    Error
    SetPortForwarding (const uint16_t port);

    Error
    DeletePortForwarding (const uint16_t port);

private:
    Error
    Connect ();

    Error
    SendMessage (const std::string &packet);

    Error
    SendDeviceMessage (const std::string &packet);

    Error
    ReadMessage (std::string &message);

    Error
    ReadResponseStatus ();

    std::string m_device_id;
    ConnectionFileDescriptor m_conn;
};

} // namespace lldb_private

#endif  // liblldb_AdbClient_h_