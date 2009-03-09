#include <limits>

#include "editenv.hpp"

using namespace editenv;

unsigned int envCut (env_scope scope, char const *name, char const *text)
{
    EnvVar var(scope, name);

    return var.cut(text);
}

void envPaste (env_scope scope, char const *name, char const *text)
{
    EnvVar var(scope, name);

    var.paste(text);
}

void envSet (env_scope scope, char const *name, char const *text)
{
    EnvVar var(scope, name);

    var.set(text);
}

void envUnset (env_scope scope, char const *name)
{
    EnvVar var(scope, name);

    var.unset();
}

char const * envValue (env_scope scope, char const *name)
{
    EnvVar var(scope, name);

    return var.value().c_str();
}

void pathAdd (env_scope scope, char const *path)
{
    size_t const sizeMax = std::numeric_limits<size_t>::max();

    size_t      length = std::string(path).length();
    size_t      pos;
    std::string value;
    EnvVar      var(scope, "TestPath");

    value = var.value();
    pos = value.find(path, 0);
    while (sizeMax != pos) {
        if (((0 == pos) ||
             (';' == value[pos - 1])) &&
            ((pos + length == value.length()) ||
             (';' == value[pos + length]))) {
            // Found the path in the "Path" environment variable already.
            return;
        }
        pos = value.find(path, pos + 1);
    }

    if (0 == var.value().length()) {
        // Nothing is in the Path environment variable yet.
        var.set(path);
    } else {
        var.paste(std::string(";") + path);
    }
}

void pathRemove (env_scope scope, char const *path)
{
    size_t const sizeMax = std::numeric_limits<size_t>::max();

    size_t      length = std::string(path).length();
    size_t      pos;
    std::string value;
    EnvVar      var(scope, "TestPath");

    value = var.value();
    pos = value.find(path);
    while (sizeMax != pos) {
        if (((0 == pos) ||
             (';' == value[pos - 1])) &&
            ((pos + length == value.length()) ||
             (';' == value[pos + length]))) {
            // Found a match in the path environment variable.
            if (0 == pos) {
                // This is the first directory in the path, so there is no
                // preceding semicolon to remove.
                if (length == value.length()) {
                    // There is no trailing semicolon, either because this is
                    // the only directory in the path.
                    value = "";
                } else {
                    // Instead of removing the preceding semicolon (which
                    // isn't there), remove the following semicolon so that the
                    // new path value doesn't begin with a semicolon.
                    value.replace(pos, length + 1, "");
                }
            } else {
                // Remove the preceding semicolon along with the path string.
                value.replace(pos - 1, length + 1, "");
            }
            pos = value.find(path, pos);
        } else {
            pos = value.find(path, pos + 1);
        }
    }

    // Set the new path environment variable.
    var.set(value);
}