#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>

//网页地址
const QString URLSTR = "http://blog.csdn.net/juzhijian/article/details/51866045";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->buttonGroup->setId(ui->radioButton,0);//设置ID
    ui->buttonGroup->setId(ui->radioButton_2,1);
    ui->buttonGroup->setId(ui->radioButton_3,2);
    //设置注册对话框标题
    vv = "2.6";
    this->setWindowTitle(QStringLiteral("方舟等级生成 V")+vv+QStringLiteral(" 64bit"));
    //检查更新
    install();
}

MainWindow::~MainWindow()
{
    delete ui;
}
//更新
void MainWindow::install()
{
    QUrl url(URLSTR);
    QNetworkAccessManager manager;
    QEventLoop loop;
    //qDebug() << "Reading code form " << URLSTR;
    //发出请求
    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    //请求结束并下载完成后，退出子事件循环
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    //开启子事件循环
    loop.exec();

    //将读到的信息写入文件
    QString code = reply->readAll();
    QString z;
    for(int i = 1;i<10000;i++)
    {
        if (code.mid(i,7)=="<title>")
        {
            z = code.mid(i+17,3);
        }
    }
    if(z!=vv)
    {
        QMessageBox message(QMessageBox::Warning,QStringLiteral("提示"),
                            QStringLiteral("检查到新版本,程序需要更新！"),
                            QMessageBox::Yes|QMessageBox::No);
        if (message.exec()==QMessageBox::Yes)
           {
            QDesktopServices::openUrl(QUrl("https://blog.juzhijian.com/"));
           }
    }
    else {
        QDesktopServices::openUrl(QUrl("https://blog.juzhijian.com/"));
    }
}

//等级函数仿官方版
QString function_level_normal(int a)
{
    long L_num1 = 10;
    QString level = "ExperiencePointsForLevel[0]=10";
    for(int i=1;i<a;i++)
    {
        L_num1 = L_num1+5*(i+1);
        level = level + ",ExperiencePointsForLevel[" + QString::number(i) + "]=" + QString::number(L_num1);
    }
    return level;
}

//等级函数自定义版
QString function_level_custom(int a,unsigned long long L_num1,int b)
{
    QString level = "ExperiencePointsForLevel[0]=10";
    for(int i=1;i<a;i++)
    {

        L_num1 = (b+100)*(L_num1+5*i)/100;
        level = level + ",ExperiencePointsForLevel[" + QString::number(i) + "]=" + QString::number(L_num1);
    }
    return level;
}

//等级函数成神版
QString function_level_god(int a)
{
    QString level = "ExperiencePointsForLevel[0]=10";
    int i;
    for(i=1;i<a;i++)
    {
        level = level + ",ExperiencePointsForLevel[" + QString::number(i) + "]=10";
    }
    return level;
}

//研究点数生成
QString function_Research(int a,int b)
{
    QString Research = "OverridePlayerLevelEngramPoints="+QString::number(b)+"\n";
    int i;
    for (i = 1; i < a; i++)
    {
        Research += "OverridePlayerLevelEngramPoints=" + QString::number(b) + "\n";
    }
    return Research;
}

//单击生成按钮生成文件
void MainWindow::on_pushButton_clicked()
{
    //文件生成
    QString sh="[/script/shootergame.shootergamemode]\n";

    //玩家等级与研究点数
    QVector<int> num(2);//声明动态数组
    num[0] = ui->lineEdit_15->text().toInt();
    num[1] = ui->lineEdit_16->text().toInt();
    //恐龙等级
    num[2] = ui->lineEdit_57->text().toInt();

    //经验设置
    quint16 pd = ui->buttonGroup->checkedId();
    QString player_leve_generate,dinosaur_level_generate;

    if(pd==0)//仿官方
    {
        player_leve_generate = function_level_normal(num[0]);
        dinosaur_level_generate = function_level_normal(num[2]);
    }
    if(pd==1)//自定义
    {
        QVector<unsigned long long> L_num(1);//经验
        L_num[0] = ui->lineEdit->text().toLongLong();
        L_num[1] = ui->lineEdit_2->text().toLongLong();
        QVector<int> num1(2);//经验系数
        num1[0] = ui->lineEdit_20->text().toInt();
        num1[1] = ui->lineEdit_19->text().toInt();

        player_leve_generate = function_level_custom(num[0],L_num[0],num1[0]);//等级，经验，系数
        dinosaur_level_generate = function_level_custom(num[2],L_num[1],num1[1]);
    }
    if(pd==2)//成神
    {
        player_leve_generate = function_level_god(num[0]);
        dinosaur_level_generate = function_level_god(num[2]);
    }

    //等级设置
    QString player_levle,dinosaur_level;//玩家 恐龙
    player_levle="LevelExperienceRampOverrides=(" + player_leve_generate + ")\n";
    dinosaur_level="LevelExperienceRampOverrides=(" + dinosaur_level_generate + ")\n";

    //玩家倍数生成
    QString player=
            "PerLevelStatsMultiplier_Player[0]=" + QString::number(ui->lineEdit_3->text().toInt()) + "\n"+
            "PerLevelStatsMultiplier_Player[1]=" + QString::number(ui->lineEdit_10->text().toInt()) + "\n"+
            "PerLevelStatsMultiplier_Player[2]=" + QString::number(ui->lineEdit_4->text().toInt()) + "\n"+
            "PerLevelStatsMultiplier_Player[3]=" + QString::number(ui->lineEdit_13->text().toInt()) + "\n"+
            "PerLevelStatsMultiplier_Player[4]=" + QString::number(ui->lineEdit_5->text().toInt()) + "\n"+
            "PerLevelStatsMultiplier_Player[5]=" + QString::number(ui->lineEdit_11->text().toInt()) + "\n" +
            "PerLevelStatsMultiplier_Player[6]=" + QString::number(ui->lineEdit_6->text().toInt()) + "\n" +
            "PerLevelStatsMultiplier_Player[7]=" + QString::number(ui->lineEdit_14->text().toInt()) + "\n" +
            "PerLevelStatsMultiplier_Player[8]=" + QString::number(ui->lineEdit_7->text().toInt()) + "\n" +
            "PerLevelStatsMultiplier_Player[9]=" + QString::number(ui->lineEdit_12->text().toInt()) + "\n" +
            "PerLevelStatsMultiplier_Player[10]=" + QString::number(ui->lineEdit_8->text().toInt()) + "\n" +
            "PerLevelStatsMultiplier_Player[11]=" + QString::number(ui->lineEdit_9->text().toInt()) + "\n";
    //恐龙倍数
    QString DinoTamed=
            "PerLevelStatsMultiplier_DinoTamed[0]=" + QString::number(ui->lineEdit_46->text().toInt()) + "\n"+
            "PerLevelStatsMultiplier_DinoTamed[1]=" + QString::number(ui->lineEdit_47->text().toInt()) + "\n"+
            "PerLevelStatsMultiplier_DinoTamed[2]=" + QString::number(ui->lineEdit_48->text().toInt()) + "\n"+
            "PerLevelStatsMultiplier_DinoTamed[3]=" + QString::number(ui->lineEdit_45->text().toInt()) + "\n"+
            "PerLevelStatsMultiplier_DinoTamed[4]=" + QString::number(ui->lineEdit_50->text().toInt()) + "\n"+
            "PerLevelStatsMultiplier_DinoTamed[5]=" + QString::number(ui->lineEdit_49->text().toInt()) + "\n" +
            "PerLevelStatsMultiplier_DinoTamed[6]=" + QString::number(ui->lineEdit_51->text().toInt()) + "\n" +
            "PerLevelStatsMultiplier_DinoTamed[7]=" + QString::number(ui->lineEdit_52->text().toInt()) + "\n" +
            "PerLevelStatsMultiplier_DinoTamed[8]=" + QString::number(ui->lineEdit_53->text().toInt()) + "\n" +
            "PerLevelStatsMultiplier_DinoTamed[9]=" + QString::number(ui->lineEdit_55->text().toInt()) + "\n";
    //野生恐龙倍数
    QString DinoWild=
            "PerLevelStatsMultiplier_DinoWild[0]=" + QString::number(ui->lineEdit_59->text().toInt()) + "\n"+
            "PerLevelStatsMultiplier_DinoWild[1]=" + QString::number(ui->lineEdit_60->text().toInt()) + "\n"+
            "PerLevelStatsMultiplier_DinoWild[2]=" + QString::number(ui->lineEdit_61->text().toInt()) + "\n"+
            "PerLevelStatsMultiplier_DinoWild[3]=" + QString::number(ui->lineEdit_58->text().toInt()) + "\n"+
            "PerLevelStatsMultiplier_DinoWild[4]=" + QString::number(ui->lineEdit_63->text().toInt()) + "\n"+
            "PerLevelStatsMultiplier_DinoWild[5]=" + QString::number(ui->lineEdit_62->text().toInt()) + "\n" +
            "PerLevelStatsMultiplier_DinoWild[6]=" + QString::number(ui->lineEdit_64->text().toInt()) + "\n" +
            "PerLevelStatsMultiplier_DinoWild[7]=" + QString::number(ui->lineEdit_65->text().toInt()) + "\n" +
            "PerLevelStatsMultiplier_DinoWild[8]=" + QString::number(ui->lineEdit_66->text().toInt()) + "\n" +
            "PerLevelStatsMultiplier_DinoWild[9]=" + QString::number(ui->lineEdit_68->text().toInt()) + "\n";
    //恐龙繁殖
    QString DinosaurDreeding=
            "MatingIntervalMultiplier=" + QString::number(ui->lineEdit_37->text().toFloat(),'f', 6) + "\n" +
            "LayEggIntervalMultiplier=" + QString::number(ui->lineEdit_38->text().toFloat(),'f', 6) + "\n" +
            "EggHatchSpeedMultiplier=" + QString::number(ui->lineEdit_39->text().toFloat(),'f', 6) + "\n" +
            "BabyMatureSpeedMultiplier=" + QString::number(ui->lineEdit_40->text().toFloat(),'f', 6) + "\n" +
            "BabyFoodConsumptionSpeedMultiplier=" + QString::number(ui->lineEdit_41->text().toFloat(),'f', 6) + "\n" +
            "BabyCuddleIntervalMultiplier=" + QString::number(ui->lineEdit_42->text().toFloat(),'f', 6) + "\n" +
            "BabyCuddleGracePeriodMultiplier=" + QString::number(ui->lineEdit_97->text().toFloat(),'f', 6) + "\n" +
            "BabyImprintingStatScaleMultiplier=" + QString::number(ui->lineEdit_43->text().toFloat(),'f', 6) + "\n" +
            "BabyCuddleLoseImprintQualitySpeedMultiplier=" + QString::number(ui->lineEdit_44->text().toFloat(),'f', 6) + "\n";
    //ui->textBrowser->setText(QString::number(aaaa));
    //生成文本
    ui->textBrowser->setText(sh + player + DinoTamed + DinoWild + player_levle + dinosaur_level
                             + function_Research(num[0],num[1]) + DinosaurDreeding);

    //导出INI文件
    QString path = QFileDialog::getSaveFileName(this, tr("Open ini"), "Game", tr("ini Files (*.ini)"));
    if(!path.isEmpty())
    {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, QString::fromLocal8Bit("提示"), QStringLiteral("不能创建文件\n%1").arg(path));
            return;
        }
        QTextStream out(&file);
        out << ui->textBrowser->toPlainText();
        file.close();
        QMessageBox::warning(this, QString::fromLocal8Bit("提示"), QStringLiteral("已经成功保存文件\n%1").arg(path));
    }
    else
    {
        QMessageBox::information(this, QString::fromLocal8Bit("警告"), QStringLiteral("您没有保存文件！"));
    }
}


void MainWindow::on_radioButton_3_clicked()
{

    ui->textBrowser->setText(QStringLiteral("您选择了成神版!"));
}

void MainWindow::on_radioButton_2_clicked()
{
    ui->textBrowser->setText(QStringLiteral("您选择了自定义版!!"));
}

void MainWindow::on_radioButton_clicked()
{
    ui->textBrowser->setText(QStringLiteral("您选择了仿官方版!!"));
}

//恐龙繁殖中宝宝成熟时间被改变
void MainWindow::on_lineEdit_40_textChanged(const QString &arg1)
{
    float AttentionIterval;
    AttentionIterval = 1/ui->lineEdit_40->text().toFloat();
    ui->lineEdit_42->setText(QString::number(AttentionIterval,'f', 6));
}
