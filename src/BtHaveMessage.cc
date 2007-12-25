/* <!-- copyright */
/*
 * aria2 - The high speed download utility
 *
 * Copyright (C) 2006 Tatsuhiro Tsujikawa
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 */
/* copyright --> */
#include "BtHaveMessage.h"
#include "PeerMessageUtil.h"
#include "Util.h"
#include "DlAbortEx.h"
#include "message.h"

BtHaveMessageHandle BtHaveMessage::create(const unsigned char* data, int32_t dataLength) {
  if(dataLength != 5) {
    throw new DlAbortEx(EX_INVALID_PAYLOAD_SIZE, "have", dataLength, 5);
  }
  int8_t id = PeerMessageUtil::getId(data);
  if(id != ID) {
    throw new DlAbortEx(EX_INVALID_BT_MESSAGE_ID, id, "have", ID);
  }
  BtHaveMessageHandle message = new BtHaveMessage();
  message->setIndex(PeerMessageUtil::getIntParam(data, 1));
  return message;
}

void BtHaveMessage::doReceivedAction() {
  peer->updateBitfield(index, 1);
}

int32_t BtHaveMessage::MESSAGE_LENGTH = 9;

const unsigned char* BtHaveMessage::getMessage() {
  if(!msg) {
    /**
     * len --- 5, 4bytes
     * id --- 4, 1byte
     * piece index --- index, 4bytes
     * total: 9bytes
     */
    msg = new unsigned char[MESSAGE_LENGTH];
    PeerMessageUtil::createPeerMessageString(msg, MESSAGE_LENGTH, 5, ID);
    PeerMessageUtil::setIntParam(&msg[5], index);
  }
  return msg;
}

int32_t BtHaveMessage::getMessageLength() {
  return MESSAGE_LENGTH;
}

string BtHaveMessage::toString() const {
  return "have index="+Util::itos(index);
}
