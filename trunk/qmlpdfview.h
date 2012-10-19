#ifndef QMLPDFVIEW_H
#define QMLPDFVIEW_H

#include <QDeclarativeItem>

namespace Poppler {
    class Document;
}

class QMLPDFView : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(int currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged)
    Q_PROPERTY(qreal zoom READ zoom WRITE setZoom NOTIFY zoomChanged)
    Q_PROPERTY(QSize pageSize READ pageSize)
    Q_PROPERTY(int numPages READ numPages)
    //PaintPageSize - height width parent
    //pos

public:
    explicit QMLPDFView(QDeclarativeItem *parent = 0);


    //Variables
    Poppler::Document* docPDF;
    QString _source;
    int _currentPage;
    qreal _zoom;
    int _numPages;
    QSize _pageSize;

    //READ Method`s
    QString source() const;
    int currentPage() const;
    qreal zoom() const;
    int numPages() const;
    QSize pageSize() const;

    //WRITE Method`s
    void setSource(const QString& url);
    void setCurrentPage(int currPage);
    void setZoom(qreal zoomPage);

    //Other Variables
    QImage page;
    int posX,posY;
    int phX,phY;


    //Other Method
    void loadPDF();
    void updatePosition();

    Q_INVOKABLE void saveToFile(const QUrl& path);

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

signals:
    void sourceChanged();
    void currentPageChanged();
    void zoomChanged();
    
public slots:
    void loadPage();
    
};

#endif // QMLPDFVIEW_H
