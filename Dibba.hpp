#ifndef DIBBA_HPP
#define DIBBA_HPP

#include <iostream>
#include <vector>

namespace Dibba {
    const int TypeDibbaHeader  = 0;
    const int TypeFile = 1;
    const int TypeDibbaEnder = 2;

    class Error {
    private:
        std::string error;
    public:
        Error(std::string p_error) : error(p_error) {}
        Error(){}
        std::string What() {
            return error;
        }
    };

    const Error ErrAlreadyCommitted("Already committed");
    const Error ErrNoFileName("No filename specified");
    const Error ErrFileAlreadyExists("File already exists");
    const Error ErrNotInit("Output buffer not initialized");
    const Error ErrMalformed("Dibba file malformed");


    template <class T> class Option {
    private:
        T some;
        bool is_some;
    public:
        Option(T);
        Option();
        void Set(T);
        bool IsSome();
        T Get();
    };

    template <class T>
    Option<T>::Option(T t) {
        Set(t);
    }

    template <class T>
    Option<T>::Option() {
        is_some = false;
    }

    template <class T>
    void Option<T>::Set(T t) {
        some = t;
        is_some = true;
    }

    template <class T>
    bool Option<T>::IsSome() {
        return is_some;
    }

    template <class T>
    T Option<T>::Get() {
        if (!is_some) {
            throw std::invalid_argument("Option<T>::Get() is called before Option value is set");
        }
        return some;
    }

    class File {
    private:
        std::string filename;
        std::istream& contents;

        Option<std::string> marshalHeaders();
    public:
        File(std::string, std::istream&);
        std::string Name();
        std::istream& GetReader();
        Option<Error> MarshalTo(std::ostream&);
    };

    typedef struct {
        std::string filename;
        int64_t start;
        int64_t length;
    } fileBounds;

    class DibbaReader {
    private:
        std::istream& rd;
        std::vector<fileBounds> fb;
        bool checkIntegrity();
    public:
        DibbaReader(std::istream&);
        Option<Error> Parse();
        Option<std::string> Read(std::string);
        std::vector<std::string> FileList();
    };

    class DibbaWriter {
    private:
        std::ostream& out;
        bool fresh;
    public:
        DibbaWriter(std::ostream&);
        Option<Error> Init();
        Option<Error> Add(File&);
        Option<Error> Commit();
    };
}
#endif // DIBBA_HPP
