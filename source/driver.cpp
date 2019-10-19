#include <sstream>
#include <lambda/lexer.hpp>
#include <driver.hpp>

extern int yy_flex_debug;

/**
 * Implement error function
 */
void lambda::Parser::error(const location_type &l, const std::string &message)
{
    auto os = std::ostringstream();
    os << l << ": " << message << std::endl;
    throw std::runtime_error(os.str());
}

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
    YY_BUFFER_STATE mState = nullptr;
};

int lambda::DriverBase::parse()
{
    auto str = std::string(std::istreambuf_iterator<char>(mStream), {});

    // https://westes.github.io/flex/manual/Multiple-Input-Buffers.html#Scanning-Strings
    // Add two YY_END_OF_BUFFER_CHAR at end as is specified by yy_scan_buffer
    // ensure two bytes if std::string does not stores the trailing NULL
    str.append("__");
    auto size = str.size();
    auto data = str.data();
    data[str.size() - 2] = data[str.size() - 1] = '\0';
    //

    // Initialize
    yy_flex_debug = mTraceScanning;
    mLocation.initialize(mStreamName.empty() ? nullptr : &mStreamName);

    // Scan the string
    auto buffer = yyBufferStateWrapper(yy_scan_buffer(data, size));

    // Parse
    auto parser = lambda::Parser(this);
    parser.set_debug_level(mTraceParsing);
    int result = parser.parse();

    // Free buffer
    buffer.dispose();
    return result;
}
