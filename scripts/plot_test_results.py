#!/usr/bin/python3

"""map
    Copyright 2023 Debby Nirwan
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
"""

import map_module
import hash_table_cpp
import pandas as pd
import plotly.express as px

insert_x = []
insert_lib_name = []
insert_time = []

insert_data = {
    "number of elements": insert_x,
    "library name": insert_lib_name,
    "time (us)": insert_time
}

at_x = []
at_lib_name = []
at_time = []

at_data = {
    "number of elements": at_x,
    "library name": at_lib_name,
    "time (us)": at_time
}

remove_x = []
remove_lib_name = []
remove_time = []

remove_data = {
    "number of elements": remove_x,
    "library name": remove_lib_name,
    "time (us)": remove_time
}

n = 10
max = 1e+06
multiplier = 2

while True:
    map = map_module.Map()
    std_map = map_module.map()
    hash_table = hash_table_cpp.HashTable()
    std_unordered_map = hash_table_cpp.unordered_map()

    insert_time_ = map_module.measure_insert(map, n)
    at_time_ = map_module.measure_at(map)
    remove_time_ = map_module.measure_remove(map, n)

    std_map_insert_time_ = map_module.measure_insert(std_map, n)
    std_map_at_time_ = map_module.measure_at(std_map)
    std_map_remove_time_ = map_module.measure_remove(std_map, n)

    hash_table_insert_time_ = hash_table_cpp.measure_insert(hash_table, n)
    hash_table_at_time_ = hash_table_cpp.measure_at(hash_table)
    hash_table_remove_time_ = hash_table_cpp.measure_remove(hash_table, n)

    std_unordered_map_insert_time_ = hash_table_cpp.measure_insert(
        std_unordered_map, n)
    std_unordered_map_at_time_ = hash_table_cpp.measure_at(std_unordered_map)
    std_unordered_map_remove_time_ = hash_table_cpp.measure_remove(
        std_unordered_map, n)

    insert_x.append(n)
    insert_lib_name.append("Map")
    insert_time.append(insert_time_)

    insert_x.append(n)
    insert_lib_name.append("std::map")
    insert_time.append(std_map_insert_time_)

    insert_x.append(n)
    insert_lib_name.append("HashTable")
    insert_time.append(hash_table_insert_time_)

    insert_x.append(n)
    insert_lib_name.append("std::unordered_map")
    insert_time.append(std_unordered_map_insert_time_)

    at_x.append(n)
    at_lib_name.append("Map")
    at_time.append(at_time_)

    at_x.append(n)
    at_lib_name.append("std::map")
    at_time.append(std_map_at_time_)

    at_x.append(n)
    at_lib_name.append("HashTable")
    at_time.append(hash_table_at_time_)

    at_x.append(n)
    at_lib_name.append("std::unordered_map")
    at_time.append(std_unordered_map_at_time_)

    remove_x.append(n)
    remove_lib_name.append("Map")
    remove_time.append(insert_time_)

    remove_x.append(n)
    remove_lib_name.append("std::map")
    remove_time.append(std_map_remove_time_)

    remove_x.append(n)
    remove_lib_name.append("HashTable")
    remove_time.append(hash_table_remove_time_)

    remove_x.append(n)
    remove_lib_name.append("std::unordered_map")
    remove_time.append(std_unordered_map_remove_time_)

    if n >= max:
        break
    else:
        n = n*multiplier

insert_data_df = pd.DataFrame(insert_data)
at_data_df = pd.DataFrame(at_data)
remove_data_df = pd.DataFrame(remove_data)

fig_insert = px.line(insert_data_df, log_x=False, markers=True, title="Insert element time performance",
                     x="number of elements", y="time (us)", color="library name")
fig_at = px.line(at_data_df, log_x=False, markers=True, title="Retrieve element time performance",
                 x="number of elements", y="time (us)", color="library name")
fig_remove = px.line(remove_data_df, log_x=False, markers=True, title="Remove element time performance",
                     x="number of elements", y="time (us)", color="library name")
fig_insert.write_image(file="insert_perf.png", scale=3.0)
fig_at.write_image(file="retrieve_perf.png", scale=3.0)
fig_remove.write_image(file="remove_perf.png", scale=3.0)
