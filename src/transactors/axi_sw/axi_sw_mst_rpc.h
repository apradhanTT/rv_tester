// SPDX-FileCopyrightText: 2026 Tenstorrent USA, Inc.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "src/transactors/axi_sw/axi.h"
#include "cvm/messenger.hpp"

// Type-erased RPC so sysmod devices (e.g. mmr_txn_router) can issue ARs on any
// axi_sw_mst without depending on rv_tester_transactions.hpp.
CVM_MESSENGER_procedure_call(axi_sw_mst_push_ar_no_id_rpc, bool(const axi::a_no_id_t& ar, axi::id_t& id));
