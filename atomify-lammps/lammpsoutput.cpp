#include "lammpsoutput.h"
#include <string>
#include <cassert>
#include <iostream>
#include <QString>
#include <QDebug>
#include <libio.h>
#include "CPcompute.h"

using namespace std;

LammpsOutput::LammpsOutput()
{
#ifdef Q_OS_LINUX
    cookie_io_functions_t funcs;
    funcs.close = LammpsOutput::clean;
    funcs.read = LammpsOutput::readLinux;
    funcs.write = LammpsOutput::writeLinux;
    funcs.seek = LammpsOutput::seekLinux;
    m_filePointer = fopencookie((void*)this, "w", funcs);
#else
    m_filePointer = funopen((const void*)this, LammpsOutput::read, LammpsOutput::write, LammpsOutput::seek, LammpsOutput::clean);
#endif

}

LammpsOutput::~LammpsOutput()
{

}


int LammpsOutput::read (void *, char *, int ) {

}
// __ssize_t __io_read_fn (void *__cookie, char *__buf, size_t __nbytes);
__ssize_t LammpsOutput::readLinux(void *, char *, size_t ) {

}

__ssize_t LammpsOutput::writeLinux(void *cookie, const char *buffer, size_t n) {
    LammpsOutput *parser = (LammpsOutput*)cookie;
    parser->parse(QString(buffer));
}

int LammpsOutput::write (void *cookie, const char *buffer, int size) {
    LammpsOutput *parser = (LammpsOutput*)cookie;
    parser->parse(QString(buffer));
}

// typedef int __io_seek_fn (void *__cookie, _IO_off64_t *__pos, int __w);
int LammpsOutput::seekLinux(void *cookie, _IO_off64_t *__pos, int __w) {
    return 0;
}

fpos_t LammpsOutput::seek (void *, fpos_t , int ) {
    fpos_t obj;
    return obj;
}

int LammpsOutput::clean (void *) {

    return 0;
}

FILE *LammpsOutput::stream()
{
    return m_filePointer;
}

void LammpsOutput::parseVectorOutput(const QString &buffer) {
    QStringList lines = buffer.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);

    unsigned int timestep = 0;
    int numberOfValues = 0;
    QVector<double> numericalOutput;

    bool isFirstLine = true;
    foreach(const QString &line, lines) {
        bool invalidLine = false;
        QStringList words = line.split(QRegExp("[ ]"), QString::SkipEmptyParts); // Split by spaces.

        bool isFirstWord = true;
        foreach(const QString &word, words) {
            bool isNumerical;
            double value = word.toDouble(&isNumerical);
            if(!isNumerical) {
                // Could not parse to double. Then this line is nothing we want.
                invalidLine = true;
                break;
            }

            if(isFirstLine) {
                // This line contains 'Timestep NumValues'
                if(isFirstWord) {
                    timestep = value;
                    isFirstWord = false;
                } else {
                    numberOfValues = value;
                }
            } else {
                // This line contains ValueId Value
                if(isFirstWord) {
                    // First word is the value id (1,2,...)
                    isFirstWord = false;
                } else {
                    // This is our value
                    numericalOutput.push_back(value);
                }
            }
        }

        if(isFirstLine) isFirstLine = false;
    }

    m_compute->setValues(numericalOutput);
}

void LammpsOutput::parseScalarOutput(const QString &buffer) {
    QStringList lines = buffer.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);

    unsigned int timestep = 0;
    foreach(const QString &line, lines) {
        bool invalidLine = false;

        QStringList words = line.split(QRegExp("[ ]"), QString::SkipEmptyParts); // Split by spaces.
        QVector<double> numericalOutput;

        bool isFirstWord = true;
        foreach(const QString &word, words) {
            bool isNumerical;
            double value = word.toDouble(&isNumerical);
            if(!isNumerical) {
                // Could not parse to double. Then this line is nothing we want.
                invalidLine = true;
                break;
            }

            if(isFirstWord) {
                // First word in fix outputs is always timestep
                timestep = value;
                isFirstWord = false;
            } else numericalOutput.push_back(value);
        }

        if(!invalidLine) {
            m_compute->setValues(numericalOutput);
        }
    }
}

void LammpsOutput::parse(QString buffer)
{
    if(m_compute->isVector()) parseVectorOutput(buffer);
    else parseScalarOutput(buffer);

}
