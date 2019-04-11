#include "effect.h"
#include<qdebug.h>
#include<QElapsedTimer>
#include<QComboBox>
Effect::Effect()

{
   tm_switch = new QTimer(this);
   mv_media = new QMovie(this);
   connect(tm_switch, SIGNAL(timeout()), this, SLOT(display()));
   opacityinterval=0.05;
   pictureeffect = new QGraphicsOpacityEffect();
   opacity = 1.0;//透明度值
   switchcase = 0;//淡入淡出中间状态标志
   flag=0;
}
void Effect::display()
{

    QElapsedTimer t;
    t.start();
    while(t.elapsed()<2)
        QCoreApplication::processEvents();
    if (opacity > 0.0)
    {

        if (switchcase == 0)
        {
            opacity -= opacityinterval;

        }
        else
        {
            opacity += opacityinterval;
        }

      setPictureOpacity();
        if (opacity == 1)
        {
            m_ui1->disconnectSlots();
            tm_switch->stop();
            switchcase = 0;
            if(m_closeway==HIDE)
            {
                m_ui1->hide();
                m_ui1->setGeometry(rect);
            }
            else
             m_ui1->close();
           m_ui2->init();
           flag=0;
        }
    }
    else
    {

        switchcase = 1;
        if (movein == NONE)//防止移出后没有移入，图片显示不出来
        {
            m_ui2->setGeometry(QRect(0, 0, m_ui2->width(), m_ui2->height()));
        }
        opacity += opacityinterval;

        setPictureOpacity();
    }
}
void Effect::setPictureOpacity()
{
    //*淡入淡出效果*************************************************//

    //pictureeffect->setOpacity(opacity);
    //m_ui1->setGraphicsEffect(pictureeffect);
    //m_ui2->setGraphicsEffect(pictureeffect);
    //*移入移出效果*************************************************//
    if (switchcase == 0)
    {//opacity:1->0
        switch(moveout)
        {
        case NONE:
            break;
        case LEFTTORIGHT:
            if(flag==0)
            {
                m_ui2->showFullScreen();
                flag=1;
            }
            m_ui1->setGeometry(QRect(m_ui1->width() * (1.0 - opacity), 0, m_ui1->width(), m_ui1->height()));

            break;
        case RIGHTTOLEFT:
            m_ui1->setGeometry(QRect(m_ui1->width() * (opacity - 1.0), 0,m_ui1->width(), m_ui1->height()));
            break;
        case UPTODOWN:
            m_ui1->setGeometry(QRect(0, m_ui1->height() * (1.0 - opacity), m_ui1->width(), m_ui1->height()));
            break;
        case DOWNTOUP:
            m_ui1->setGeometry(QRect(0, m_ui1->height() * (opacity - 1.0), m_ui1->width(), m_ui1->height()));
            break;
        default:
            break;
        }
    }
    else
    {//opacity:0->1
        switch(movein)
        {
        case NONE:
            break;
        case LEFTTORIGHT:
            if(flag==0)
            {

               if(m_style==WAIYE)
               {
                m_ui2->setAutoFillBackground(true);
                QPalette palette;
                m_ui2->showFullScreen();
                palette.setBrush(m_ui2->backgroundRole(),QBrush(QPixmap(":/image/picture/qietu/底纹.jpg").scaled(m_ui2->size(),Qt::IgnoreAspectRatio,                                                                                                 Qt::SmoothTransformation)));
                m_ui2->setPalette(palette);
                flag=1;
               }
               else if(m_style==NEIYE)
               {
                   m_ui2->setAutoFillBackground(true);
                   QPalette palette;
                   m_ui2->showFullScreen();
                   palette.setBrush(m_ui2->backgroundRole(),QBrush(QPixmap(":/image/picture/qietu/neiye.jpg").scaled(m_ui2->size(),Qt::IgnoreAspectRatio,                                                                                                 Qt::SmoothTransformation)));
                   m_ui2->setPalette(palette);
                   flag=1;
               }
               else if(m_style==HUIYE)
               {
                   m_ui2->setAutoFillBackground(true);
                   QPalette palette;
                   m_ui2->showFullScreen();
                   palette.setColor(QPalette::Background,QColor(235,235,235));
                   //palette.setBrush(m_ui2->backgroundRole(),QBrush(QPixmap(":/image/picture/qietu/neiye.jpg").scaled(m_ui2->size(),Qt::IgnoreAspectRatio,                                                                                                 Qt::SmoothTransformation)));
                   m_ui2->setPalette(palette);
                   flag=1;
               }
            }
            m_ui2->setGeometry(QRect(m_ui1->width() * (opacity - 1.0), 0, m_ui1->width(), m_ui1->height()));

            break;
        case RIGHTTOLEFT:
            if(flag==0)
            {
                m_ui2->showFullScreen();
                flag=1;
            }
            m_ui2->setGeometry(QRect(m_ui2->width() * (1.0 - opacity), 0, m_ui2->width(),m_ui2->height()));

            break;
        case UPTODOWN:
           m_ui2->setGeometry(QRect(0, m_ui2->height() * (opacity - 1.0), m_ui2->width(), m_ui2->height()));
            break;
        case DOWNTOUP:
            m_ui2->setGeometry(QRect(0, m_ui2->height() * (1.0 - opacity), m_ui2->width(), m_ui2->height()));
            break;
        default:
            break;
        }
    }
}
void Effect::playPicttureinLabel(QStringList &m_picturePathlist, QLabel *m_label)
{
    if(m_picturePathlist.size()<1)
       return;
    m_label->setMovie(mv_media);
    mv_media->stop();
    mv_media->setFileName(m_picturePathlist.at(0));
    mv_media->start();
}
void Effect::setOpacityInterval(double opacityinterval)//设置透明度渐变粒度
{
    this->opacityinterval = opacityinterval;
}

void Effect::setSwitchMsec(long switchmsec)//设置渐变计时器时间与停留时间
{
    this->switchmsec = switchmsec;

}
void Effect::setMoveWay(MOVEWAY movein, MOVEWAY moveout)//设置移入移出的方向
{
    this->movein = movein;
    this->moveout = moveout;
}
void Effect::begin(CommonWidget *widget1, CommonWidget *widget2, MOVEWAY movein, MOVEWAY moveout, CLOSEWAY closeway, BACKGROUNDSTYLE backgroundstyle)
{
    m_ui1=widget1;
    m_ui2=widget2;
    rect=m_ui1->geometry();
    m_closeway=closeway;
    m_style=backgroundstyle;
    setMoveWay(movein,moveout);
    tm_switch->start(5);

}
QTComboBoxButton::QTComboBoxButton(QWidget *parent /* = 0 */) :
    QLineEdit(parent)
{
}

QTComboBoxButton::~QTComboBoxButton()
{

}

void QTComboBoxButton::mousePressEvent(QMouseEvent * e)
{
    Q_UNUSED(e);
    QComboBox* combo=dynamic_cast<QComboBox*>(parent());
    if(combo)
        combo->showPopup();
}
