#include <iostream>
#include <QCoreApplication>
#include <QtNetwork/QDnsLookup>
#include <QtNetwork/QHostAddress>
#include <QDebug>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //For tests:
    const char* addr = "www.black.skydns.ru";
    //QDnsLookup *dns = new QDnsLookup(QDnsLookup::TXT, "vk.com");
    QDnsLookup *dns = new QDnsLookup(QDnsLookup::TXT, addr);
    QObject::connect(dns, &QDnsLookup::finished, [&dns, &addr]()
                     {
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
                            std::cout << addr << "has no TXT record" << "\n";
                         }
                         QCoreApplication::quit();
                     });
    dns->lookup();

    return a.exec();
}
