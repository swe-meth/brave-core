/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ledger/internal/core/sql_store.h"

#include <string>
#include <utility>

#include "bat/ledger/internal/core/bat_ledger_test.h"

namespace ledger {

class SQLStoreTest : public BATLedgerTest {};

TEST_F(SQLStoreTest, BindValues) {
  auto bindings =
      SQLStore::BindValues(true, int32_t(2202), 0.008, nullptr, "hello",
                           int64_t(64), SQLStore::Bind(nullptr));

  ASSERT_EQ(bindings.size(), size_t(7));

  EXPECT_EQ(bindings[0]->index, 0);
  EXPECT_EQ(bindings[0]->value->get_bool_value(), true);

  EXPECT_EQ(bindings[1]->index, 1);
  EXPECT_EQ(bindings[1]->value->get_int_value(), 2202);

  EXPECT_EQ(bindings[2]->index, 2);
  EXPECT_EQ(bindings[2]->value->get_double_value(), 0.008);

  EXPECT_EQ(bindings[3]->index, 3);
  EXPECT_EQ(bindings[3]->value->get_null_value(), 0);

  EXPECT_EQ(bindings[4]->index, 4);
  EXPECT_EQ(bindings[4]->value->get_string_value(), "hello");

  EXPECT_EQ(bindings[5]->index, 5);
  EXPECT_EQ(bindings[5]->value->get_int64_value(), 64);

  EXPECT_EQ(bindings[6]->index, 6);
  EXPECT_EQ(bindings[6]->value->get_null_value(), 0);
}

TEST_F(SQLStoreTest, CreateCommand) {
  auto command = SQLStore::CreateCommand("insert into T values (?, ?, ?)", "a",
                                         1, int64_t(64));

  EXPECT_EQ(command->type, mojom::DBCommand::Type::RUN);
  EXPECT_EQ(command->command, "insert into T values (?, ?, ?)");

  auto& bindings = command->bindings;
  ASSERT_EQ(command->bindings.size(), size_t(3));

  EXPECT_EQ(bindings[0]->index, 0);
  EXPECT_EQ(bindings[0]->value->get_string_value(), "a");

  EXPECT_EQ(bindings[1]->index, 1);
  EXPECT_EQ(bindings[1]->value->get_int_value(), 1);

  EXPECT_EQ(bindings[2]->index, 2);
  EXPECT_EQ(bindings[2]->value->get_int64_value(), 64);
}

TEST_F(SQLStoreTest, GetColumnTypes) {
  auto types = SQLStore::GetColumnTypes<int32_t, int64_t, bool, std::string>();
  ASSERT_EQ(types.size(), size_t(4));

  EXPECT_EQ(types[0], mojom::DBCommand::RecordBindingType::INT_TYPE);
  EXPECT_EQ(types[1], mojom::DBCommand::RecordBindingType::INT64_TYPE);
  EXPECT_EQ(types[2], mojom::DBCommand::RecordBindingType::BOOL_TYPE);
  EXPECT_EQ(types[3], mojom::DBCommand::RecordBindingType::STRING_TYPE);
}

}  // namespace ledger
