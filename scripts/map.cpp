#include "map.hpp"
#include <map>
#include <pybind11/pybind11.h>
#include <time.h>

namespace py = pybind11;
using MapInt = Map<int, int>;
using MapIntDouble = Map<int, double>;
using mapInt = std::map<int, int>;

struct ProfileInsertResults {
    MapIntDouble map_time;
    MapIntDouble std_map_time;
};

ProfileInsertResults ProfileInsert(const std::size_t n)
{
    clock_t start, end;
    double cpu_time_used;
    MapInt map;
    mapInt std_map;
    ProfileInsertResults result;

    for (std::size_t i = 0; i < n; i++) {
        start = clock();
        std_map.insert({ i, i * 5 });
        end = clock();
        cpu_time_used = end - start;
        result.std_map_time.Insert(i, cpu_time_used);
    }

    for (std::size_t i = 0; i < n; i++) {
        start = clock();
        map.Insert(i, i * 5);
        end = clock();
        cpu_time_used = end - start;
        result.map_time.Insert(i, cpu_time_used);
    }

    return result;
}

double MeasureInsert(MapInt& map, const std::size_t n)
{
    clock_t start, end;

    start = clock();
    for (std::size_t i = 0; i < n; i++) {
        map.Insert(i, i * 5);
    }
    end = clock();

    return (end - start);
}

double MeasureAt(MapInt& map)
{
    clock_t start, end;
    int x;

    start = clock();
    for (std::size_t i = 0; i < map.Size(); i++) {
        x = map.At(i);
    }
    end = clock();

    x++;

    return (end - start);
}

double MeasureRemove(MapInt& map, const std::size_t n)
{
    clock_t start, end;

    start = clock();
    for (std::size_t i = 0; i < n; i++) {
        map.Remove(i);
    }
    end = clock();

    return (end - start);
}

double MeasureInsert(mapInt& map, const std::size_t n)
{
    clock_t start, end;

    start = clock();
    for (std::size_t i = 0; i < n; i++) {
        map.insert({ i, i * 5 });
    }
    end = clock();

    return (end - start);
}

double MeasureAt(mapInt& map)
{
    clock_t start, end;
    int x;

    start = clock();
    for (std::size_t i = 0; i < map.size(); i++) {
        x = map.at(i);
    }
    end = clock();

    x++;

    return (end - start);
}

double MeasureRemove(mapInt& map, const std::size_t n)
{
    clock_t start, end;

    start = clock();
    for (std::size_t i = 0; i < n; i++) {
        map.erase(i);
    }
    end = clock();

    return (end - start);
}

PYBIND11_MODULE(map_module, m)
{
    py::class_<MapInt>(m, "Map")
        .def(py::init())
        .def("at", &MapInt::At)
        .def("insert", &MapInt::Insert)
        .def("size", &MapInt::Size)
        .def("save_tree", &MapInt::SaveTree);

    py::class_<MapIntDouble>(m, "MapIntDouble")
        .def(py::init())
        .def("at", &MapIntDouble::At)
        .def("insert", &MapIntDouble::Insert)
        .def("size", &MapIntDouble::Size)
        .def("save_tree", &MapIntDouble::SaveTree);

    py::class_<mapInt>(m, "map").def(py::init());

    py::class_<ProfileInsertResults>(m, "ProfileInsertResults")
        .def(py::init())
        .def_readwrite("map_time", &ProfileInsertResults::map_time)
        .def_readwrite("std_map_time", &ProfileInsertResults::std_map_time);

    m.def("profile_insert",
          static_cast<ProfileInsertResults (*)(const std::size_t)>(&ProfileInsert));

    m.def("measure_insert", static_cast<double (*)(MapInt&, const std::size_t)>(&MeasureInsert));
    m.def("measure_insert", static_cast<double (*)(mapInt&, const std::size_t)>(&MeasureInsert));

    m.def("measure_at", static_cast<double (*)(MapInt&)>(&MeasureAt));
    m.def("measure_at", static_cast<double (*)(mapInt&)>(&MeasureAt));

    m.def("measure_remove", static_cast<double (*)(MapInt&, const std::size_t)>(&MeasureRemove));
    m.def("measure_remove", static_cast<double (*)(mapInt&, const std::size_t)>(&MeasureRemove));
}
