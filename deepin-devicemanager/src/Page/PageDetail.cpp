#include "PageDetail.h"
#include "TextBrowser.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPalette>
#include <DApplicationHelper>
#include <DApplication>
#include <DStyle>
#include <QDebug>
#include <QScrollBar>

DWIDGET_USE_NAMESPACE

DetailButton::DetailButton(const QString &txt)
    : DCommandLinkButton(txt)
{

}

void DetailButton::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.setOpacity(1);
    painter.setClipping(true);
    QRect rect = this->rect();

    // 获取调色板
    DApplicationHelper *dAppHelper = DApplicationHelper::instance();
    DPalette palette = dAppHelper->applicationPalette();

    // 获取窗口当前的状态,激活，禁用，未激活
    DPalette::ColorGroup cg;
    DWidget *wid = DApplication::activeWindow();
    if (wid/* && wid == this*/) {
        cg = DPalette::Active;
    } else {
        cg = DPalette::Inactive;
    }

    // 开始绘制边框 *********************************************************
    // 计算绘制区域
    QBrush bgBrush(palette.color(cg, DPalette::Base));
    painter.fillRect(rect, bgBrush);

    painter.restore();
    DCommandLinkButton::paintEvent(e);
}

DetailSeperator::DetailSeperator(DWidget *parent)
    : DWidget(parent)
{
    setFixedHeight(18);
}

void DetailSeperator::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.setOpacity(1);
    painter.setClipping(true);
    QRect rect = this->rect();

    // 获取调色板
    DApplicationHelper *dAppHelper = DApplicationHelper::instance();
    DPalette palette = dAppHelper->applicationPalette();

    // 分割线两端到边框的边距
    int spacing = 20;

    // 获取窗口当前的状态,激活，禁用，未激活
    DPalette::ColorGroup cg;
    DWidget *wid = DApplication::activeWindow();
    if (wid/* && wid == this*/) {
        cg = DPalette::Active;
    } else {
        cg = DPalette::Inactive;
    }

    // 清除背景色颜色
    QBrush clearBrush(palette.color(cg, DPalette::Base));
    painter.fillRect(rect, clearBrush);

    // 开始绘制边框 *********************************************************
    // 计算绘制区域
    rect.setX(rect.x() + spacing);
    rect.setWidth(rect.width() - spacing);
    rect.setY(rect.y() + height() - 2);
    QBrush bgBrush(palette.color(cg, DPalette::FrameShadowBorder));
    painter.fillRect(rect, bgBrush);

    painter.restore();
    DWidget::paintEvent(e);
}

ScrollAreaWidget::ScrollAreaWidget(DWidget *parent)
    : DWidget(parent)
{
}

void ScrollAreaWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.setOpacity(1);
    painter.setClipping(true);
    QRect rect = this->rect();

    // 获取调色板
    DApplicationHelper *dAppHelper = DApplicationHelper::instance();
    DPalette palette = dAppHelper->applicationPalette();

    // 获取窗口当前的状态,激活，禁用，未激活
    DPalette::ColorGroup cg;
    DWidget *wid = DApplication::activeWindow();
    if (wid/* && wid == this*/) {
        cg = DPalette::Active;
    } else {
        cg = DPalette::Inactive;
    }

    // 清除背景色颜色
    QBrush clearBrush(palette.color(cg, DPalette::Base));
    painter.fillRect(rect, clearBrush);
}

PageDetail::PageDetail(QWidget *parent)
    : Dtk::Widget::DWidget(parent)
    , mp_ScrollAreaLayout(new QVBoxLayout(this))
    , mp_ScrollArea(new QScrollArea(this))
{
    setContentsMargins(0, 0, 0, 0);
    QVBoxLayout *hLayout = new QVBoxLayout(this);
    hLayout->setContentsMargins(0, 0, 0, 0);

    // 设置scrollarea的属性
    mp_ScrollArea->setWidgetResizable(true);
    mp_ScrollArea->setFrameShape(QFrame::NoFrame);
    mp_ScrollArea->setContentsMargins(0, 0, 0, 0);
    mp_ScrollAreaLayout->setContentsMargins(0, 0, 0, 0);
    mp_ScrollAreaLayout->setSpacing(0);


    // 设置ScrollArea里面的widget,这个widget是必须要的
    ScrollAreaWidget *widget = new ScrollAreaWidget(this);
    widget->setContentsMargins(0, 0, 0, 0);
    widget->setLayout(mp_ScrollAreaLayout);
    mp_ScrollArea->setWidget(widget);
    hLayout->addWidget(mp_ScrollArea);
    setLayout(hLayout);
}

void PageDetail::showDeviceInfo(const QList<DeviceBaseInfo *> &lstInfo)
{
    // Clear widgets first
    clearWidget();

    // Create widgets for showing device info
    foreach (auto device, lstInfo) {
        if (!device) {continue;}
        TextBrowser *txtBrowser = new TextBrowser(this);
        txtBrowser->showDeviceInfo(device);
        addWidgets(txtBrowser);
    }
    mp_ScrollAreaLayout->addStretch();
}

void PageDetail::showInfoOfNum(int index)
{
    if (index >= m_ListHlayout.size()
            || index >= m_ListTextBrowser.size()
            || index >= m_ListDetailButton.size()
            || index >= m_ListDetailSeperator.size()) {
        return;
    }
    int value = 0;
    for (int i = 0; i <= index - 1; i++) {
        value += m_ListTextBrowser[i]->height();
        value += m_ListDetailButton[i]->height();
        value += m_ListDetailSeperator[i]->height();
    }
    mp_ScrollArea->verticalScrollBar()->setValue(value);
}

void PageDetail::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.setOpacity(1);
    painter.setClipping(true);
    QRect rect = this->rect();

    // 获取调色板
    DApplicationHelper *dAppHelper = DApplicationHelper::instance();
    DPalette palette = dAppHelper->applicationPalette();

    // 获取系统默认的圆角半径
//    QStyleOptionFrame option;
//    initStyleOption(&option);
//    DStyle *style = dynamic_cast<DStyle *>(DApplication::style());
//    int radius = style->pixelMetric(DStyle::PM_FrameRadius, &option);
    int radius = 8;

    // 获取窗口当前的状态,激活，禁用，未激活
    DPalette::ColorGroup cg;
    DWidget *wid = DApplication::activeWindow();
    if (wid/* && wid == this*/) {
        cg = DPalette::Active;
    } else {
        cg = DPalette::Inactive;
    }

    // 开始绘制边框 *********************************************************
    // 计算绘制区域
    QPainterPath paintPath;
    paintPath.addRoundedRect(rect, radius, radius);
    QBrush bgBrush(palette.color(cg, DPalette::Base));
    painter.fillPath(paintPath, bgBrush);

    painter.restore();
    DWidget::paintEvent(e);
}

void PageDetail::addWidgets(TextBrowser *widget)
{
    mp_ScrollAreaLayout->addWidget(widget);

    // 添加按钮
    QHBoxLayout *vLayout = new QHBoxLayout(this);
    vLayout->addSpacing(34);
    DetailButton *button = new DetailButton(tr("Details"));
    connect(button, &DetailButton::clicked, this, &PageDetail::slotBtnClicked);
    vLayout->addWidget(button);
    vLayout->addStretch(-1);
    mp_ScrollAreaLayout->addLayout(vLayout);

    // 添加分割线
    DetailSeperator *seperator = new DetailSeperator(widget);
    mp_ScrollAreaLayout->addWidget(seperator);

    m_ListTextBrowser.append(widget);
    m_ListHlayout.append(vLayout);
    m_ListDetailButton.append(button);
    m_ListDetailSeperator.append(seperator);
}

void PageDetail::clearWidget()
{
    foreach (auto widget, m_ListTextBrowser) {
        delete widget;
    }
    foreach (auto widget, m_ListHlayout) {
        delete widget;
    }
    foreach (auto widget, m_ListDetailButton) {
        delete widget;
    }
    foreach (auto widget, m_ListDetailSeperator) {
        delete widget;
    }

    m_ListTextBrowser.clear();
    m_ListHlayout.clear();
    m_ListDetailButton.clear();
    m_ListDetailSeperator.clear();

    // 删除最后的一个弹簧
    QLayoutItem *layoutItem = mp_ScrollAreaLayout->itemAt(0);
    mp_ScrollAreaLayout->removeItem(layoutItem);
    delete  layoutItem;
}

void PageDetail::slotBtnClicked()
{
    DetailButton *button = qobject_cast<DetailButton *>(sender());
    int index = 0;
    foreach (DetailButton *b, m_ListDetailButton) {
        if (button == b) {
            break;
        }
        index++;
    }

    TextBrowser *browser = m_ListTextBrowser[index];
    browser->updateInfo();
    if (button->text() == tr("Details")) {
        button->setText(tr("Put away"));
    } else {
        button->setText(tr("Details"));
    }
}