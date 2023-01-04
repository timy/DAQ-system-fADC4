#pragma once

#include <iostream>
#include <vector>
#include <string.h>

class LogLines {
public:

    LogLines() {}

    virtual ~LogLines() {
        clear();
    }

    void append(const wchar_t* str) {
        // 寻找所有断行符 \n，按 \n 拆分成条目并插入 v
        const wchar_t* p, * p0 = str;
        int64_t n;
        p = wcschr(p0, '\n');
        while (p != NULL) {
            n = p - p0;
            insertSingleLine(p0, n);
            p0 = p + 1;
            p = wcschr(p0, '\n');
        }
        // 处理结束所有 \n 后，查看剩余字符串结束前是否有内容，若有，则向 v 插入该条目 
        p = wcschr(p0, '\0');
        n = p - p0;
        if (n > 0) {
            insertSingleLine(p0, n);
        }
    }

    void clear() {
        while (!v.empty()) {
            delete[] v.back();
            v.pop_back();
        }
    }

    void show() {
        for (auto i : v) {
            std::cout << i << std::endl;
        }
    }

    size_t size() {
        return v.size();
    }

    const wchar_t* operator [] (int i) {
        return v[i];
    }

private:
    std::vector<wchar_t*> v;

    void insertSingleLine(const wchar_t* p0, int64_t n) {
        v.push_back(new wchar_t[n + 1]);
        wcsncpy_s(v.back(), n+1, p0, n);
        // v.back()[n] = '\0'; // not needed when using 'wcsnncpy_s'
    }
};


static int test_LogLines() {
    using std::cout;
    using std::endl;

    wchar_t str1[] = L"this is a test str.\nWe will show the result.\nHey!";
    wchar_t str2[] = L"[LOG] 1\n\n[LOG] 2\n";
    cout << "total string length: " << wcslen(str1) << endl;

    LogLines txtlines;
    txtlines.append(str1);
    txtlines.append(str2);
    txtlines.show();

    cout << "No. lines: " << txtlines.size() << endl;

    cout << "###########################" << endl;
    for (int i = 0; i < txtlines.size(); i++)
        cout << txtlines[i] << endl;

    return 0;
}
