#include "library.h"

#include <cassert>
#include <iostream>
#include <fstream>
#include <utility>

using namespace std;


void Document::updateTitle(const string &newTitle) {
    _title.clear();
    _title = newTitle;
}

void Document::updateYear(int newYear) { _year = newYear; }
void Document::updateQuantity(int newQuantity) { _quantity = newQuantity; }
string Document::getTitle() { return _title; }
int Document::getYear() { return _year; }
int Document::getQuantity() { return _quantity; }

bool Document::borrowDoc() {
    if (_quantity > 0) {
        _quantity--;
        return true;
    }
    return false;
}

void Document::returnDoc() { _quantity++; }

Novel::Novel(const string &title, string author, int year, int quantity):_author(std::move(author)) {
    _title = title;

    _year = year;
    _quantity = quantity;
}

Novel::~Novel() {
    _author.clear();
    _title.clear();
}

DocType Novel::getDocType() { return DOC_NOVEL; }

void Novel::print() {
    cout<< "Novel, title: "<< _title << ", author: "<< _author << ", year: " <<_year <<", quantity: "<< _quantity<< std::endl;

}

void Novel::updateAuthor(const string &newAuthor) {
    _author.clear();
    _author = newAuthor;


}

string Novel::getAuthor() { return _author; }

Comic::Comic(const string &title, string author, int issue, int year,
             int quantity):_author(std::move(author)),_issue(issue) {
    _title = title;

    _year = year;
    _quantity = quantity;

}

Comic::~Comic() {
    _author.clear();
    _title.clear();
}

DocType Comic::getDocType() { return DOC_COMIC; }

void Comic::print() {
    cout<< "Comic, title: "<< _title << ", author: "<< _author << ", issue: " << _issue <<", year: " <<_year <<", quantity: "<< _quantity<< std::endl;

}
void Comic::updateAuthor(const string &newAuthor) {
    _author.clear();
    _author = newAuthor;

}

void Comic::updateIssue(int newIssue) { _issue = newIssue; }
string Comic::getAuthor() { return _author; }
int Comic::getIssue() { return _issue; }

Magazine::Magazine(const string &title, int issue, int year, int quantity):_issue(issue) {
    _title = title;

    _year = year;
    _quantity = quantity;

}

Magazine::~Magazine() {
    _title.clear(); }

DocType Magazine::getDocType() { return DOC_MAGAZINE; }

void Magazine::print() {
    cout<<"Magazine, title: "<< _title<< ", issue: "<< _issue << ", year: " <<_year <<", quantity: "<< _quantity<< std::endl;
}

void Magazine::updateIssue(int newIssue) { _issue = newIssue; }
int Magazine::getIssue() { return _issue; }

Library::Library() : _docs_sz(0){}

bool Library::addDocument(DocType t, const string &title, const string &author,
                         int issue, int year, int quantity) {
    shared_ptr<Document> doc;
    switch (t) {
        case DOC_NOVEL: {
            doc = make_shared<Novel>(title, author, year, quantity);
            break;
        }

        case DOC_COMIC: {
            doc = make_shared<Comic>(title, author, issue, year, quantity);
            break;
        }

        case DOC_MAGAZINE: {
            doc = make_shared<Magazine>(title, issue, year, quantity);
            break;
        }

        default:
            return false;
    }
    return addDocument(doc);
}

bool Library::addDocument(shared_ptr<Document> doc) {
    for (const shared_ptr<Document>& a:_docs)
        if (doc->getTitle() == a->getTitle())
            return false;
    _docs.push_back(doc);
    _docs_sz++;
    return true;
}

bool Library::delDocument(const string &title) {
    int index = -1;
    int i = 0;
    for (const shared_ptr<Document>& a:_docs) {
        if (a->getTitle() == title) {
            index = i;
            break;
        }
        i++;
    }

    if (index != -1) {
        _docs.erase(_docs.begin() + index);
        _docs_sz--;
        return true;
    }

    return false;
}

int Library::countDocumentOfType(DocType t) {
    int res = 0;

    for (const shared_ptr<Document>& a:_docs)
        if (a->getDocType() == t)
            res++;

    return res;
}

shared_ptr<Document> Library::searchDocument(const string &title) {
    for (const shared_ptr<Document>& a:_docs)
        if (a->getTitle()== title)
            return a;

    return nullptr;
}

void Library::print() {
    for (const shared_ptr<Document>& a:_docs)
        a->print();
}

bool Library::borrowDoc(const string &title) {
    shared_ptr<Document> d = searchDocument(title);
    if (d)
        return d->borrowDoc();
    return false;
}

bool Library::returnDoc(const string &title) {
    shared_ptr<Document> d = searchDocument(title);
    if (d) {
        d->returnDoc();
        return true;
    }
    return false;
}

bool Library::dumpCSV(const string &filename) {
    ofstream fd;
    fd.open(filename );
    if (!fd.is_open() )
        return false;

    for (shared_ptr< Document> d : _docs) {
        switch (d->getDocType()) {
            case DOC_NOVEL: {
                Novel *n = dynamic_cast<Novel *>(d.get());
                fd<< "novel,"<<n->getTitle()<<","<<n->getAuthor()<<",,"<<n->getYear()<<","<<n->getQuantity()<<endl;
                break;
            }

            case DOC_COMIC: {
                Comic *c = dynamic_cast<Comic *>(d.get());
                fd<<"comic,"<<c->getTitle()<<","<<c->getAuthor()<<","<<c->getIssue()<<","<<c->getYear()<<","<<c->getQuantity()<<endl;
                break;
            }

            case DOC_MAGAZINE: {
                Magazine *m = dynamic_cast<Magazine *>(d.get());
                fd<<"magazine,"<<m->getTitle()<<",,"<<m->getIssue()<<","<<m->getYear()<<","<<m->getQuantity()<<endl;
                break;
            }

            default:
                return false;
        }
    }

    fd.close();
    return true;
}
