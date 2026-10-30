#include "../engine.cpp"
#include <cassert>
int main() {
    controlall::Voice voice = {};
    controlall::Shared shared;
    opxy::i32 parameters[8] = {};
    opxy::u32 pitch[opxy::block_frames] = {};
    opxy::i32 output[opxy::block_frames];
    for (auto& sample : output) sample = 123;
    controlall::initialize(voice, shared, 1);
    controlall::start(voice, shared, 1);
    controlall::render(voice, shared, 1, output, parameters, pitch);
    for (auto sample : output) assert(sample == 0);
    controlall::destroy(voice, shared, 1);
}
