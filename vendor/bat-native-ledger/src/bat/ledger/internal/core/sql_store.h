/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_VENDOR_BAT_NATIVE_LEDGER_SRC_BAT_LEDGER_INTERNAL_CORE_SQL_STORE_H_
#define BRAVE_VENDOR_BAT_NATIVE_LEDGER_SRC_BAT_LEDGER_INTERNAL_CORE_SQL_STORE_H_

#include <string>
#include <utility>
#include <vector>

#include "bat/ledger/internal/core/async_result.h"
#include "bat/ledger/internal/core/bat_ledger_context.h"
#include "bat/ledger/public/interfaces/ledger.mojom.h"

namespace ledger {

class SQLStore : public BATLedgerContext::Component {
 public:
  static const size_t kComponentKey;

  explicit SQLStore(BATLedgerContext* context);
  ~SQLStore() override;

  using CommandResult = AsyncResult<mojom::DBCommandResponsePtr>;
  CommandResult RunTransaction(mojom::DBTransactionPtr transaction);
  CommandResult RunTransaction(mojom::DBCommandPtr command);

  template <typename... Args>
  CommandResult Execute(const std::string& sql, Args&&... args) {
    return RunTransaction(CreateCommand(sql, std::forward<Args>(args)...));
  }

  template <typename... Args>
  CommandResult Query(const std::string& sql) {
    return Query<Args...>(CreateCommand(sql));
  }

  template <typename... Args>
  CommandResult Query(mojom::DBCommandPtr command) {
    DCHECK(command);
    command->type = mojom::DBCommand::Type::READ;
    command->record_bindings = GetColumnTypes<Args...>();
    return RunTransaction(std::move(command));
  }

  template <typename T>
  static mojom::DBValuePtr Bind(T&& value) {
    auto db_value = mojom::DBValue::New();
    SetDBValue(db_value.get(), std::forward<T>(value));
    return db_value;
  }

  template <typename... Args>
  static std::vector<mojom::DBCommandBindingPtr> BindValues(Args&&... args) {
    std::vector<mojom::DBCommandBindingPtr> bindings;
    AddBindings(&bindings, std::forward<Args>(args)...);
    return bindings;
  }

  template <typename... Args>
  static std::vector<mojom::DBCommand::RecordBindingType> GetColumnTypes() {
    std::vector<mojom::DBCommand::RecordBindingType> column_types;
    AddColumnTypes<decltype(column_types), Args...>(&column_types);
    return column_types;
  }

  template <typename... Args>
  static mojom::DBCommandPtr CreateCommand(const std::string& sql,
                                           Args&&... args) {
    auto command = mojom::DBCommand::New();
    command->type = mojom::DBCommand::Type::RUN;
    command->command = sql;
    command->bindings = BindValues(std::forward<Args>(args)...);
    return command;
  }

 protected:
  virtual CommandResult RunTransactionImpl(mojom::DBTransactionPtr transaction);

 private:
  static void SetDBValue(mojom::DBValue* db_value, double value);
  static void SetDBValue(mojom::DBValue* db_value, int32_t value);
  static void SetDBValue(mojom::DBValue* db_value, int64_t value);
  static void SetDBValue(mojom::DBValue* db_value, bool value);
  static void SetDBValue(mojom::DBValue* db_value, const char* value);
  static void SetDBValue(mojom::DBValue* db_value, const std::string& value);
  static void SetDBValue(mojom::DBValue* db_value, std::nullptr_t);
  static void SetDBValue(mojom::DBValue* db_value, mojom::DBValuePtr);

  template <typename T>
  static mojom::DBCommand::RecordBindingType GetColumnType() = delete;

  template <typename T>
  static void AddColumnTypes(T*) {}

  template <typename T, typename U, typename... Args>
  static void AddColumnTypes(T* column_types) {
    column_types->push_back(GetColumnType<U>());
    AddColumnTypes<T, Args...>(column_types);
  }

  template <typename T>
  static void AddBindings(T* bindings) {}

  template <typename T, typename U, typename... Args>
  static void AddBindings(T* bindings, U&& value, Args&&... args) {
    auto binding = mojom::DBCommandBinding::New();
    binding->index = bindings->size();
    binding->value = Bind(std::forward<U>(value));
    bindings->push_back(std::move(binding));
    AddBindings(bindings, std::forward<Args>(args)...);
  }
};

template <>
mojom::DBCommand::RecordBindingType SQLStore::GetColumnType<std::string>() {
  return mojom::DBCommand::RecordBindingType::STRING_TYPE;
}

template <>
mojom::DBCommand::RecordBindingType SQLStore::GetColumnType<bool>() {
  return mojom::DBCommand::RecordBindingType::BOOL_TYPE;
}

template <>
mojom::DBCommand::RecordBindingType SQLStore::GetColumnType<double>() {
  return mojom::DBCommand::RecordBindingType::DOUBLE_TYPE;
}

template <>
mojom::DBCommand::RecordBindingType SQLStore::GetColumnType<int32_t>() {
  return mojom::DBCommand::RecordBindingType::INT_TYPE;
}

template <>
mojom::DBCommand::RecordBindingType SQLStore::GetColumnType<int64_t>() {
  return mojom::DBCommand::RecordBindingType::INT64_TYPE;
}

}  // namespace ledger

#endif  // BRAVE_VENDOR_BAT_NATIVE_LEDGER_SRC_BAT_LEDGER_INTERNAL_CORE_SQL_STORE_H_
