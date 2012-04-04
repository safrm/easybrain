#ifndef WEBLINK_H
#define WEBLINK_H
#include <QLabel>
#include <QUrl>

class WebLink : public QLabel
{
    Q_OBJECT
public:
    explicit WebLink(const QString &text, const QUrl &url, QWidget *parent = 0);
protected :
  void mousePressEvent(QMouseEvent *ev);

signals:
  void clicked();

private slots:
    void openUrl();
private:
    QUrl m_url;
};

#endif // WEBLINK_H
