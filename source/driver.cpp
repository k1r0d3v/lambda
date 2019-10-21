#include <sstream>
#include <lexer.hpp>
#include <parser.hpp>
#include <driver.hpp>

extern int yy_flex_debug;

/**
 * Needed to avoid memory leaks when a exception is thrown in parse
 */
class yyBufferStateWrapper
{
public:
    explicit yyBufferStateWrapper(YY_BUFFER_STATE state)
            : mState(state) { }

    ~yyBufferStateWrapper() { dispose(); }

    void dispose() {
        if (mState != nullptr) {
            yy_delete_buffer(mState);
            mState = nullptr;
        }
    }

private:
    YY_BUFFER_STATE mState;
};

void yy::Driver::parse(std::istream &stream, void *data, std::string name, bool traceScan, bool traceParse)
{
    Driver driver;

    // Initialize
    driver.mData = data;
    driver.mLocation.initialize(name.empty() ? nullptr : &name);

    auto str = std::string(std::istreambuf_iterator<char>(stream), {});

    // https://westes.github.io/flex/manual/Multiple-Input-Buffers.html#Scanning-Strings
    // Add two YY_END_OF_BUFFER_CHAR at end as is specified by yy_scan_buffer
    // ensure two bytes if std::string does not stores the trailing NULL
    str.append("__");
    auto streamSize = str.size();
    auto streamData = str.data();
    streamData[str.size() - 2] = streamData[str.size() - 1] = '\0';
    //

    // Scan the string
    auto buffer = yyBufferStateWrapper(yy_scan_buffer(streamData, streamSize));

    // Parser instance
    auto parser = yy::Parser(&driver);

    // Set debug levels
    yy_flex_debug = traceScan;
    parser.set_debug_level(traceParse);

    // Parse
    parser.parse();

    // Free buffer
    buffer.dispose();
}