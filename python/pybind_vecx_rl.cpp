#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

#include "environment.hpp"

namespace py = pybind11;

using namespace vecx_rl;

PYBIND11_MODULE(pyvecx_rl, m)
{
    m.doc() = "Provides interface for interaction with a vectrex emulator for reinforcement learning agents";
    py::register_local_exception<unsupported_rom>(m, "unsupported_rom", PyExc_RuntimeError);

    py::class_<action>(m, "action")
        .def(py::init<>())
        .def_property("event", &action::get_action, &action::set_action);

    py::class_<environment>(m, "environment")
        .def(py::init<uint64_t, bool, bool, bool, const std::optional<std::pair<int, int>>&>(),
             py::arg("frames_per_step") = 1,
             py::arg("real_time") = true,
             py::arg("enable_window") = true,
             py::arg("enable_sound") = false,
             py::arg("image_dims") = std::optional<std::pair<int, int>>())
        .def("load_rom", &environment::load_rom)
        .def("step", &environment::step)
        .def("reset", &environment::reset)
        .def("start_new_game", &environment::start_new_game)
        .def("is_game_finished", &environment::is_game_finished)
        .def("get_reward", &environment::get_reward)
        .def("get_legal_actions", &environment::get_legal_actions)
        .def("get_image", &environment::get_image);
}
