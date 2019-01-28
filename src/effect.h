#ifndef EFFECT_H
#define EFFECT_H

#include<qtimer.h>
#include<QMouseEvent>
#include<qpoint.h>
#include<QLabel>
#include<QMovie>
#include<QGraphicsOpacityEffect>
#include<QHBoxLayout>
#include<QGridLayout>
#include<QtMultimedia/qmultimedia.h>
#include<QLineEdit>
enum MOVEWAY//平移类型
{
    NONE,
    LEFTTORIGHT,
    RIGHTTOLEFT,
    UPTODOWN,
    DOWNTOUP
};
enum CLOSEWAY
{
    CLOSE,
    HIDE
};
enum   BACKGROUNDSTYLE
{
    NEIYE,
    WAIYE,
    HUIYE
};
class Effect:public QObject
{
    Q_OBJECT
private:
    int rec;
    int flag;
    QPoint offset;
    qint16 blockSize;
    QString path;
    QWidget *m_ui1;
    QWidget *m_ui2;
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
public:
    QRect rect;
    QTimer    *tm_switch;
    explicit Effect();
   // ~Effect();
    MOVEWAY   movein;
    MOVEWAY   moveout;
    CLOSEWAY m_closeway;
    BACKGROUNDSTYLE m_style;
    QLabel    *lb_picture;//示意图
    QMovie    *mv_media;//多媒体播放
    long      switchmsec;//示意图切换定时器切换间隔(毫秒数)
    QGraphicsOpacityEffect *pictureeffect;//示意图透明度样式
    double    opacity;//透明度值
    double    opacityinterval;//淡入淡出透明度变化间隔
    long      switchcase;//淡入淡出中间状态标志
 public:
    void setPictureOpacity();//设置透明度与位移效果
    void setOpacityInterval(double opacityinterval = 0.05);//设置透明度渐变粒度
    void setMoveWay(MOVEWAY movein, MOVEWAY moveout);
    void setSwitchMsec(long switchmsec = 30);
    void playPicttureinLabel(QStringList &m_picturePathlist, QLabel *m_label);
    void
    begin(QWidget *widget1, QWidget *widget2, MOVEWAY movein, MOVEWAY moveout, CLOSEWAY closeway, BACKGROUNDSTYLE backgroundstyle=NEIYE);
 public slots:
    //void receiveMesg();
    void display();
protected:
   // virtual void resizeEvent(QResizeEvent *event);

};
class QTComboBoxButton : public QLineEdit
{
    Q_OBJECT
public:
    QTComboBoxButton(QWidget *parent = 0);
    ~QTComboBoxButton();

protected:
    void mousePressEvent(QMouseEvent *);
};
#endif // EFFECT_H
