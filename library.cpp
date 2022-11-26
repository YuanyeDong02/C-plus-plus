#include "library.h"

#include <fcntl.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cassert>
#include <iostream>
#include <fstream>

using namespace std;


void Document::updateTitle(const char *newTitle) {
    free(_title);
    _title = (char *)malloc((strlen(newTitle) + 1) * sizeof(char));
    assert(_title);
    strcpy(_title, newTitle);
}

void Document::updateYear(int newYear) { _year = newYear; }
void Document::updateQuantity(int newQuantity) { _quantity = newQuantity; }
char *Document::getTitle() { return _title; }
int Document::getYear() { return _year; }
int Document::getQuantity() { return _quantity; }

int Document::borrowDoc() {
    if (_quantity > 0) {
        _quantity--;
        return 1;
    }
    return 0;
}

void Document::returnDoc() { _quantity++; }

Novel::Novel(const char *title, const char *author, int year, int quantity) {
    _title = (char *)malloc((strlen(title) + 1) * sizeof(char));
    _author = (char *)malloc((strlen(author) + 1) * sizeof(char));
    assert(_title && _author);

    strcpy(_title, title);
    strcpy(_author, author);
    _year = year;
    _quantity = quantity;
}

Novel::~Novel() {
    free(_author);
    free(_title);
}

DocType Novel::getDocType() { return DOC_NOVEL; }

void Novel::print() {
    cout<< "Novel, title: "<< _title << ", author: "<< _author << ", year: " <<_year <<", quantity: "<< _quantity<< std::endl;

}

void Novel::updateAuthor(const char *newAuthor) {
    free(_author);
    _author = (char *)malloc((strlen(newAuthor) + 1) * sizeof(char));
    assert(_author);
    strcpy(_author, newAuthor);
}

char *Novel::getAuthor() { return _author; }

Comic::Comic(const char *title, const char *author, int issue, int year,
             int quantity) {
    _title = (char *)malloc((strlen(title) + 1) * sizeof(char));
    _author = (char *)malloc((strlen(author) + 1) * sizeof(char));
    assert(_author && _title);
    strcpy(_title, title);
    strcpy(_author, author);
    _year = year;
    _quantity = quantity;
    _issue = issue;
}

Comic::~Comic() {
    free(_author);
    free(_title);
}

DocType Comic::getDocType() { return DOC_COMIC; }

void Comic::print() {
    cout<< "Comic, title: "<< _title << ", author: "<< _author << ", issue: " << _issue <<", year: " <<_year <<", quantity: "<< _quantity<< std::endl;

}
void Comic::updateAuthor(const char *newAuthor) {
    free(_author);
    _author = (char *)malloc((strlen(newAuthor) + 1) * sizeof(char));
    assert(_author);
    strcpy(_author, newAuthor);
}

void Comic::updateIssue(int newIssue) { _issue = newIssue; }
char *Comic::getAuthor() { return _author; }
int Comic::getIssue() { return _issue; }

Magazine::Magazine(const char *title, int issue, int year, int quantity) {
    _title = (char *)malloc((strlen(title) + 1) * sizeof(char));
    assert(_title);

    strcpy(_title, title);
    _year = year;
    _quantity = quantity;
    _issue = issue;
}

Magazine::~Magazine() { free(_title); }

DocType Magazine::getDocType() { return DOC_MAGAZINE; }

void Magazine::print() {
    cout<<"Magazine, title: "<< _title<< ", issue: "<< _issue << ", year: " <<_year <<", quantity: "<< _quantity<< std::endl;
}

void Magazine::updateIssue(int newIssue) { _issue = newIssue; }
int Magazine::getIssue() { return _issue; }

Library::Library() { _docs_sz = 0; };

int Library::addDocument(DocType t, const char *title, const char *author,
                         int issue, int year, int quantity) {
    Document *d;
    switch (t) {
        case DOC_NOVEL: {
            d = (Document *)new Novel(title, author, year, quantity);
            break;
        }

        case DOC_COMIC: {
            d = (Document *)new Comic(title, author, issue, year, quantity);
            break;
        }

        case DOC_MAGAZINE: {
            d = (Document *)new Magazine(title, issue, year, quantity);
            break;
        }

        default:
            return 0;
    }
    return addDocument(d);
}

int Library::addDocument(Document *d) {
    for (int i = 0; i < _docs_sz; i++)
        if (!strcmp(_docs[i]->getTitle(), d->getTitle()))
            return 0;

    _docs[_docs_sz++] = d;
    return 1;
}

int Library::delDocument(const char *title) {
    int index = -1;
    for (int i = 0; i < _docs_sz; i++)
        if (!strcmp(_docs[i]->getTitle(), title)) {
            index = i;
            break;
        }

    if (index != -1) {
        free(_docs[index]);
        for (int i = index + 1; i < _docs_sz; i++)
            _docs[i - 1] = _docs[i];
        _docs_sz--;
        return 1;
    }

    return 0;
}

int Library::countDocumentOfType(DocType t) {
    int res = 0;

    for (int i = 0; i < _docs_sz; i++)
        if (_docs[i]->getDocType() == t)
            res++;

    return res;
}

Document *Library::searchDocument(const char *title) {
    for (int i = 0; i < _docs_sz; i++)
        if (!strcmp(_docs[i]->getTitle(), title))
            return _docs[i];

    return NULL;
}

void Library::print() {
    for (int i = 0; i < _docs_sz; i++)
        _docs[i]->print();
}

int Library::borrowDoc(const char *title) {
    Document *d = searchDocument(title);
    if (d)
        return d->borrowDoc();
    return 0;
}

int Library::returnDoc(const char *title) {
    Document *d = searchDocument(title);
    if (d) {
        d->returnDoc();
        return 1;
    }
    return 0;
}

int Library::dumpCSV(const char *filename) {
    char line[128];
    int bytes_written;
    ofstream fd;
    fd.open(filename , ios::out | ios::trunc);
    if (!fd.is_open() )
        return 0;

    for (int i = 0; i < _docs_sz; i++) {
        Document *d = _docs[i];
        switch (d->getDocType()) {
            case DOC_NOVEL: {
                Novel *n = (Novel *)d;
                sprintf(line, "novel,%s,%s,,%d,%d\n", n->getTitle(), n->getAuthor(),
                        n->getYear(), n->getQuantity());
                break;
            }

            case DOC_COMIC: {
                Comic *c = (Comic *)d;
                sprintf(line, "comic,%s,%s,%d,%d,%d\n", c->getTitle(), c->getAuthor(),
                        c->getIssue(), c->getYear(), c->getQuantity());
                break;
            }

            case DOC_MAGAZINE: {
                Magazine *m = (Magazine *)d;
                sprintf(line, "magazine,%s,,%d,%d,%d\n", m->getTitle(), m->getIssue(),
                        m->getYear(), m->getQuantity());

                break;
            }

            default:
                return 0;
        }

        fd << line;
    }

    fd.close();
    return 1;
}
