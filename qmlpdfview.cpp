#include "qmlpdfview.h"
#include "poppler-qt4.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QFile>
#include <QDebug>

QMLPDFView::QMLPDFView(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    _source(""), _currentPage(0),
    _zoom(1.0),
    posX(0), posY(0),
    phX(QApplication::desktop()->physicalDpiX()), phY(QApplication::desktop()->physicalDpiY()),
    _numPages(0)
{
    connect(this,SIGNAL(currentPageChanged()),this,SLOT(loadPage()));
    connect(this,SIGNAL(zoomChanged()),this,SLOT(loadPage()));
    setFlag(ItemHasNoContents,false);
}

QString QMLPDFView::source() const
{
    return _source;
}

int QMLPDFView::currentPage() const
{
    return _currentPage;
}

qreal QMLPDFView::zoom() const
{
    return _zoom;
}

int QMLPDFView::numPages() const
{
    return _numPages;
}

QSize QMLPDFView::pageSize() const
{
    return _pageSize;
}

void QMLPDFView::setSource(const QString &url)
{
    if((!url.isEmpty()) && (url != _source)) {
        _source = url;
        _currentPage = 0;
        loadPDF();
        emit sourceChanged();
    } else {
        qDebug()<<"Please enter valid url. Your url: "<<url;
        return;
    }
}

void QMLPDFView::setCurrentPage(int currPage)
{
    if((currPage > (numPages() - 1)) || (currPage < 0)) {
        qDebug()<<"Bad page. Please re-enter!"<<"\nInput: "<<currPage<<"\nMax page: "<<docPDF->numPages();
        return;
    }

    if(currPage != _currentPage) {
        _currentPage = currPage;
        emit currentPageChanged();
    } else {
        qDebug()<<"Equally("<<_currentPage<<","<<currPage<<")";
        return;
    }
}

void QMLPDFView::setZoom(qreal zoomPage)
{
    //Maximum zoom 2.0 or less

    if(zoomPage != _zoom) {
        _zoom = zoomPage;
        emit zoomChanged();
    } else {
        qDebug()<<"Equally("<<_zoom<<","<<zoomPage<<")";
        return;
    }
}


void QMLPDFView::loadPDF()
{
    //Create flag isLoad

    if(!QFile::exists(_source)) {
        qDebug()<<"Not found file: "<<_source;
        return;
    }

    docPDF = Poppler::Document::load(_source);
    _numPages = docPDF->numPages();

    loadPage();
}

void QMLPDFView::updatePosition()
{
    if(!_pageSize.isValid()) {
        qDebug()<<"Not valid parent size.";
        return;
    }

    int parentWidth = width();
    int parentHeight = height();

    if(_pageSize.width() < parentWidth)
        posX = (parentWidth - _pageSize.width()) / 2;
    else
        posX = 0;

    if(_pageSize.height() < parentHeight)
        posY = (parentHeight - _pageSize.height()) / 2;
    else
        posY = 0;
}

void QMLPDFView::saveToFile(const QUrl &path)
{
    qDebug()<<"Save to file!";
    page.save(path.toString(),"JPG");
}

void QMLPDFView::loadPage()
{
    int resX = _zoom * phX;
    int resY = _zoom * phY;

    page = docPDF->page(_currentPage)->renderToImage(resX,resY);

    _pageSize = page.size();

    update();
}

void QMLPDFView::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    painter->setClipRect(0,0,width(),height());

    updatePosition();

    if(!page.isNull())
        painter->drawImage(QPoint(posX,posY),page, childrenRect().toRect());
}

