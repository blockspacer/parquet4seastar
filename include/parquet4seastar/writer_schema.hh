/*
 * This file is open source software, licensed to you under the terms
 * of the Apache License, Version 2.0 (the "License").  See the NOTICE file
 * distributed with this work for additional information regarding copyright
 * ownership.  You may not use this file except in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
/*
 * Copyright (C) 2020 ScyllaDB
 */

#pragma once

#include <parquet4seastar/logical_type.hh>

namespace parquet4seastar::writer_schema {

using node = std::variant<
    struct primitive_node,
    struct struct_node,
    struct list_node,
    struct map_node
>;

struct primitive_node {
    std::string name;
    bool optional;
    logical_type::logical_type logical_type;
    std::optional<uint32_t> type_length;
    format::Encoding::type encoding;
    format::CompressionCodec::type compression;
};

struct list_node {
    std::string name;
    bool optional;
    std::unique_ptr<node> element;
};

struct map_node {
    std::string name;
    bool optional;
    std::unique_ptr<node> key;
    std::unique_ptr<node> value;
};

struct struct_node {
    std::string name;
    bool optional;
    std::vector<node> fields;
};

struct schema {
    std::vector<node> fields;
};

struct write_schema_result {
    std::vector<format::SchemaElement> elements;
    std::vector<std::vector<std::string>> leaf_paths;
};

write_schema_result write_schema(const schema& root);

} // namespace parquet4seastar::writer_schema
