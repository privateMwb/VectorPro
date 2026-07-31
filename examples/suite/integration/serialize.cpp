// Vector serialization example.
//
// Demonstrates:
// - writing a Vector<int> out to a std::ostringstream
// - reading a Vector<int> back in from a std::istringstream
// - round-tripping through <sstream> with no external dependencies

#include <support/framework.h>

#include <sstream>

using namespace VectorPro;

// Writes a Vector<int> as space-separated values.
static std::string serialize(const Vector<int>& v) {
    std::ostringstream out;
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (i > 0)
            out << ' ';
        out << v[i];
    }
    return out.str();
}

// Reads space-separated values back into a Vector<int>.
static Vector<int> deserialize(const std::string& text) {
    std::istringstream in(text);
    Vector<int> v;

    int value;
    while (in >> value)
        v.push_back(value);

    return v;
}

static void run_examples() {
    // Serialize a Vector to a plain string.
    setTitle("Serialize");

    Vector<int> v{10, 20, 30, 40, 50};
    std::string text = serialize(v);

    std::cout << "Original size : " << v.size() << "\n";
    std::cout << "Serialized    : \"" << text << "\"\n\n";

    // Deserialize the string back into a new Vector.
    setTitle("Deserialize");

    Vector<int> restored = deserialize(text);

    std::cout << "Restored size : " << restored.size() << "\n";
    std::cout << "Elements      : ";
    for (int x : restored)
        std::cout << x << " ";
    std::cout << "\n\n";

    // Round-trip check.
    setTitle("Round-Trip Check");

    bool matches = (v == restored);
    std::cout << "Original == Restored : " << matches << "\n";
}

REGISTER_EXAMPLE_SUITE();
