/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ledger/internal/core/sql_store.h"

#include <string>
#include <utility>
#include <vector>

namespace ledger {

const size_t SQLStore::kComponentKey = BATLedgerContext::ReserveComponentKey();

SQLStore::SQLStore(BATLedgerContext* context) : Component(context) {}

SQLStore::~SQLStore() = default;

void SQLStore::SetDBValue(mojom::DBValue* db_value, double value) {
  db_value->set_double_value(value);
}

void SQLStore::SetDBValue(mojom::DBValue* db_value, int32_t value) {
  db_value->set_int_value(value);
}

void SQLStore::SetDBValue(mojom::DBValue* db_value, int64_t value) {
  db_value->set_int64_value(value);
}

void SQLStore::SetDBValue(mojom::DBValue* db_value, bool value) {
  db_value->set_bool_value(value);
}

void SQLStore::SetDBValue(mojom::DBValue* db_value, const char* value) {
  db_value->set_string_value(value);
}

void SQLStore::SetDBValue(mojom::DBValue* db_value, const std::string& value) {
  db_value->set_string_value(value);
}

void SQLStore::SetDBValue(mojom::DBValue* db_value, std::nullptr_t) {
  db_value->set_null_value(0);
}

void SQLStore::SetDBValue(mojom::DBValue* db_value, mojom::DBValuePtr value) {
  *db_value = *value;
}

SQLStore::CommandResult SQLStore::RunTransaction(
    mojom::DBTransactionPtr transaction) {
  return RunTransactionImpl(std::move(transaction));
}

SQLStore::CommandResult SQLStore::RunTransaction(mojom::DBCommandPtr command) {
  auto transaction = mojom::DBTransaction::New();
  transaction->commands.push_back(std::move(command));
  return RunTransactionImpl(std::move(transaction));
}

SQLStore::CommandResult SQLStore::RunTransactionImpl(
    mojom::DBTransactionPtr transaction) {
  DCHECK(transaction);

  CommandResult::Resolver resolver;
  context()->GetLedgerClient()->RunDBTransaction(
      std::move(transaction),
      [resolver](mojom::DBCommandResponsePtr response) mutable {
        DCHECK(response);
        resolver.Complete(std::move(response));
      });

  return resolver.result();
}

}  // namespace ledger
