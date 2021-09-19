#include <iostream>
#include <QCoreApplication>
#include <QtNetwork/QDnsLookup>
#include <QtNetwork/QHostAddress>
#include <QDebug>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    const char* addr = "black.skydns.ru";

    //For tests:
    //QDnsLookup *dns = new QDnsLookup(QDnsLookup::TXT, "vk.com");
    QDnsLookup *dns = new QDnsLookup(QDnsLookup::TXT, addr);
    QObject::connect(dns, &QDnsLookup::finished, [&dns, &addr]()
                     {
                         if (dns->error() && dns->error() != QDnsLookup::InvalidReplyError)
                         {
                            std::cout << dns->errorString().toStdString() << "\n";
                            QCoreApplication::quit();
                            return;
                         }

                         const auto records = dns->textRecords();
                         if (records.size() > 0)
                         {
                             for (auto &record : records)
                             {
                                 QStringList values;
                                 foreach (const auto &item, record.values())
                                 {
                                     values.append(QString::fromLocal8Bit(item));
                                 }

                                 foreach (const auto &item, values)
                                 {
                                     std::cout << record.name().toStdString() << " descriptive text \"";
                                     std::cout << item.toStdString() << "\" ";
                                 }
                                 std::cout << "\n";
                             }
                         } else
                         {
                            std::cout << addr << " has no TXT record" << "\n";
                         }
                         QCoreApplication::quit();
                     });
    dns->lookup();

    return a.exec();
}
