#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
using namespace std;
sql::Driver *driver;
sql::Connection *con;
sql::Statement *stmt;
sql::ResultSet *res;
sql::PreparedStatement *pstmt;
void arayüz();
//Veri tabaný baðlantýsýný saðlar ve veritabaný yaratma iþlemlerini yapar.
void database_connection()
{
	/* Baðlantý oluþtur */
	driver = get_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3306", "root", "1997ege");
	//	Veritabaný yaratma
	
	//stmt = con->createStatement();
	//stmt->execute("CREATE DATABASE havayolu");
	
	// Veritabanýyla baðlantý kurma 
	con->setSchema("havayolu");
}
//Tablolarý yaratmamýzý saðlar.
void Tablo_Yaratma() {
	stmt = con->createStatement();
	stmt->execute("CREATE TABLE IF NOT EXISTS yolcular(Tckn varchar(12) NOT NULL,primary key(Tckn),Yolcu_Adi varchar(20) NOT NULL,Adres varchar(45) NOT NULL,Email varchar(30) NOT NULL,Sehir varchar(15) NOT NULL)");
	stmt->execute("CREATE TABLE IF NOT EXISTS ucaklar(Kno varchar(7) NOT NULL,primary key(Kno),Model varchar(15) NOT NULL,Kapasite int NOT NULL)");
	stmt->execute("CREATE TABLE IF NOT EXISTS seferler(Tarih varchar(10) NOT NULL,Kno varchar(7) NOT NULL,FOREIGN KEY(Kno) REFERENCES ucaklar(Kno),Baslangic int NOT NULL,Bitis int NOT NULL,saat varchar(6) NOT NULL,primary key(Tarih,Kno))");
	stmt->execute("CREATE TABLE IF NOT EXISTS yolcu(Tarih varchar(10) NOT NULL,FOREIGN KEY(Tarih) REFERENCES seferler(Tarih) ON DELETE CASCADE,Kno varchar(7) NOT NULL,FOREIGN KEY(Kno) REFERENCES seferler(Kno) ON DELETE CASCADE,Tckn varchar(12) NOT NULL,FOREIGN KEY(Tckn) REFERENCES yolcular(Tckn) ON DELETE CASCADE,Baslangic int NOT NULL,Bitis int NOT NULL,PRIMARY KEY(Baslangic,Bitis,Tckn,Kno,Tarih))");
}
//Ucaklar tablosuna yeni ucak eklememizi saðlar.
void Yeni_Ucak() {
		try {
			string Kno;
			char model[8];
			int kapasite;
			cout << "\t\t\t\t\t\tYENI UCAK EKLEME\n\n\n";
			printf("Kuyruk No:");
			cin >> Kno;
			cin.clear();
			cin.ignore();
			printf("Model:");
			cin >> model;
			printf("Kapasite:");
			cin >> kapasite;
			pstmt = con->prepareStatement("INSERT INTO ucaklar(Kno,Model,Kapasite) VALUES(?,?,?)");
			pstmt->setString(1, Kno);
			pstmt->setString(2, model);
			pstmt->setInt(3, kapasite);
			pstmt->executeUpdate();
		}
		catch (sql::SQLException &e) {
			cout << "# Hatali veri girisi(Muhtemelen var olan bir Kuyruk No girdiniz.)\n";
		}
}
//Seferler tablosuna yeni sefer eklememizi saðlar.
void Yeni_Sefer() {
	try {
		string Kno;
		int baslangic;
		int bitis;
		char tarih[10];
		char saat[6];
		cout << "\t\t\t\t\t\tYENI SEFER EKLEME\n\n\n";
		printf("Baslangic:");
		cin >> baslangic;
		cin.clear();
		cin.ignore();
		printf("Bitis:");
		cin >> bitis;
		cin.clear();
		cin.ignore();
		printf("Tarih:");
		cin >> tarih;
		printf("Saat:");
		cin >> saat;
		printf("Kuyruk No:");
		cin >> Kno;
		cin.clear();
		cin.ignore();
		pstmt = con->prepareStatement("INSERT INTO seferler(Baslangic,Bitis,Tarih,saat,Kno) VALUES(?,?,?,?,?)");
		pstmt->setInt(1, baslangic);
		pstmt->setInt(2, bitis);
		pstmt->setString(3, tarih);
		pstmt->setString(4, saat);
		pstmt->setString(5, Kno);
		pstmt->executeUpdate();
	}
	catch (sql::SQLException &e) {
		cout << "# Hatali veri girisi(Muhtemelen olmayan bir veri girmeye calistiniz.)\n";
	}
}
//Yolcular tablosuna yeni yolcu eklememizi saðlar.
void Yeni_Yolcu() {
	try {
		char Tckn[12];
		char yolcuadi[20];
		char adres[45];
		char email[20];
		char sehir[15];
		cout << "\t\t\t\t\t\tYENI YOLCU EKLEME\n\n\n";
		printf("Tckn:");
		cin >> Tckn;
		cin.clear();
		cin.ignore();
		printf("Yolcu Adi:");
		cin.get(yolcuadi, 20);
		cin.clear();
		cin.ignore();
		printf("Sehir:");
		cin >> sehir;
		cin.clear();
		cin.ignore();
		printf("Adres:");
		cin.get(adres, 45);
		printf("E-mail:");
		cin >> email;
		pstmt = con->prepareStatement("INSERT INTO yolcular(Tckn,Yolcu_Adi,Adres,Email,Sehir) VALUES(?,?,?,?,?)");
		pstmt->setString(1, Tckn);
		pstmt->setString(2, yolcuadi);
		pstmt->setString(3, adres);
		pstmt->setString(4, email);
		pstmt->setString(5, sehir);
		pstmt->executeUpdate();
	}
	catch (sql::SQLException &e) {
		cout << "#Hatali veri girisi";	
	}
}
//Yolcular tablosunu listeler.
void yolcu_listesi() {
	size_t i;
	size_t count;
	cout << "\t\t\t\t\t\tYOLCU LISTESI\n\n\n";
	pstmt = con->prepareStatement("SELECT * FROM yolcular ");
	res = pstmt->executeQuery();
	count = res->rowsCount();//Satýr sayýmýzý sayar döngü için
	cout << "TC No:\t\t\tYolcu Adi\t\tAdres\t\t\tSehir\t\t\tEmail\n";
	for (i = 0; i <count; i++) {
		res->next();
		cout << res->getString("Tckn") << "\t\t" << res->getString("Yolcu_Adi") << "\t\t"  << res->getString("Adres") << "\t\t" << res->getString("Sehir") << "\t\t" << res->getString("Email") << endl;
	}
}
//Seferler tablosunu listeler.
void sefer_listesi() {
	size_t i;
	size_t count;
	cout << "\t\t\t\t\t\tSEFER LISTESI\n\n\n";
	pstmt = con->prepareStatement("SELECT * FROM seferler ");
	res = pstmt->executeQuery();
	count = res->rowsCount();//Satýr sayýmýzý sayar döngü için
	cout << "Baslangic\t\t\tBitis\t\t\tTarih\t\t\tSaat\t\t\tKno\n";
	for (i = 0; i <count; i++) {
		res->next();
		cout << res->getString("Baslangic") << "\t\t" << "\t\t" << res->getString("Bitis") << "\t\t\t" << res->getString("Tarih") << "\t\t" << res->getString("saat") << "\t\t\t" << res->getString("Kno") << "\t\t" << endl;
	}
}
//Uçaklar tablosunu listeler.
void ucak_listesi() {
	size_t i;
	size_t count;
	cout << "\t\t\t\t\t\tUCAK LISTESI\n\n\n";
	pstmt = con->prepareStatement("SELECT * FROM ucaklar ");
	res = pstmt->executeQuery();
	count = res->rowsCount();//Satýr sayýmýzý sayar döngü için
	cout << "Kno\t\tModel\t\tKapasite\n";
	for (i = 0; i <count; i++) {
		res->next();
		cout << res->getString("Kno") << "\t\t" << res->getString("Model") << "\t\t" << res->getString("Kapasite") << "\t\t" << endl;
	}
}
//Yolcu tablomuza yeni veri girer,bilet satma iþlemini yapar.
void bilet_satma() {
	try {
		int baslangic;
		int bitis;
		int biletsayisi;
		char tarih[10];
		char Tckn[12];
		string Kno;
		cout << "\t\t\t\t\t\tBILET SATMA EKRANI\n\n\n";
		printf("Baslangic:");
		cin >> baslangic;
		cin.clear();
		cin.ignore();
		printf("Bitis:");
		cin >> bitis;
		cin.clear();
		cin.ignore();
		printf("Tarih:");
		cin >> tarih;
		cin.clear();
		cin.ignore();
		printf("Bilet Sayisi:");
		cin >> biletsayisi;
		cin.clear();
		cin.ignore();
		pstmt = con->prepareStatement("select * from seferler where Tarih = ?  and Baslangic = ? and Bitis = ?");
		pstmt->setString(1, tarih);
		pstmt->setInt(2, baslangic);
		pstmt->setInt(3, bitis);
		res = pstmt->executeQuery();
		size_t count;
		count = res->rowsCount();//Satýr sayýmýzý sayar döngü için
		cout << "Tarih\t\tBaslangic\t\tBitis\t\tKuyruk NO\n";
		for (int i = 0; i <count; i++) {
			res->next();
			cout << res->getString("Tarih") << "\t\t" << res->getString("Baslangic") << "\t\t" << res->getString("Bitis") << "\t\t" << res->getString("Kno") << "\t\t" << endl;
		}
		printf("Hangi ucusa bilet almak istersiniz:");
		cin >> Kno;
		cin.clear();
		cin.ignore();
		for (int i = 0; i < biletsayisi; i++)
		{
			printf("%d.Tckn:", i + 1);
			cin >> Tckn;
			cin.clear();
			cin.ignore();
			pstmt = con->prepareStatement("INSERT INTO yolcu(Tckn,Baslangic,Bitis,Tarih,Kno) VALUES(?,?,?,?,?)");
			pstmt->setString(1, Tckn);
			pstmt->setInt(2, baslangic);
			pstmt->setInt(3, bitis);
			pstmt->setString(4, tarih);
			pstmt->setString(5, Kno);
			pstmt->executeUpdate();
		}
	}
	catch (sql::SQLException &e) {
		cout << "# Hatali veri girisi(Muhtemelen olmayan bir veri girmeye calistiniz.)\n";
	}
}
//Yolcu tablosunda silme iþlemi yapar ve bileti iptal eder.
void Bilet_Iptal() {
		string girilentc;
		string girilenKno;
		char girilentarih[10];
		cout << "\t\t\t\t\t\tBILET IPTAL EKRANI\n\n\n";
		printf("Iptal edilecek biletin Tc Kimlik No:");
		cin >> girilentc;
		printf("Iptal edilecek biletin Tarihi:");
		cin >> girilentarih;
		printf("Iptal edilecek biletin ucaginin Kuyruk Numarasi:");
		cin >> girilenKno;
		pstmt = con->prepareStatement("DELETE FROM yolcu where Tckn = " + girilentc + " AND Tarih = " + girilentarih + " AND Kno ='" + girilenKno + "'");
		res = pstmt->executeQuery();
}
//Uçuþ seferini iptal ederek.Bilet ve sefer silme iþlemlerini yapar.
void ucus_iptal() {
		string Kno;
		string tarih;
		int baslangic;
		int bitis;
		cout << "\t\t\t\t\t\tUCUS IPTAL EKRANI\n\n\n";
		printf("Iptal edilmesi gereken ucusun Baslangic Noktasi:");
		cin >> baslangic;
		printf("Iptal edilmesi gereken ucusun Bitisi:");
		cin >> bitis;
		printf("Iptal edilmesi gereken ucusun Tarihi:");
		cin >> tarih;
		printf("Iptal edilmesi gereken ucusun Kuyruk No'su:");
		cin >> Kno;
		pstmt = con->prepareStatement("DELETE FROM seferler where Tarih = " + tarih + " AND Baslangic = " + to_string(baslangic) + " AND Bitis = " + to_string(bitis) + " AND Kno ='" + Kno + "'");
		res = pstmt->executeQuery();
	}
//Menüye dönmek isteyip istemediðimizi sorar.
void Menuye_Don() {
	char don;
	printf("Menuye donmek istiyor musunuz? Y/N:");
	cin >> don;
	switch (don)
	{
	case 'Y' :
	case 'y':
		system("cls");
		arayüz();
	case 'N':
	case 'n':
		exit;
	}
}
//Tc kimlik numarasý sorgulayarak o yolcunun yaptýðý seferleri görmemizi saðlar.
void yolcu_sorgula() {
	cout << "\t\t\t\t\t\tYOLCU SORGULAMA\n\n\n";
	try {
		string Tckn;
		size_t count;
		printf("Tckn:");
		cin >> Tckn;
		pstmt = con->prepareStatement("select * from yolcu where Tckn IN (" + Tckn + ")");
		res = pstmt->executeQuery();
		count = res->rowsCount();//Satýr sayýmýzý sayar döngü için
		cout << "Tarih\t\t\tKno\t\tTckn\t\tBaslangic\tBitis\n";
		for (int i = 0; i <count; i++) {
			res->next();
			cout << res->getString("Tarih") << "\t\t" << res->getString("Kno") << "\t\t" << res->getString("Tckn") << "\t" << res->getString("Baslangic") << "\t\t" << res->getString("Bitis") << endl;
		}
	}
	catch (sql::SQLException &e) {
		cout << "# Lutfen 11 haneli Tc'nizi giriniz.";
	}
}
//Tarih,Kuyruk numarasý sorgulayarak o seferdeki yolcularýn bilgilerini görmemizi saðlar.
void Sefer_Sorgula() {
	cout << "\t\t\t\t\t\tSEFER SORGULAMA\n\n\n";
	try {
		string Kno;
		string tarih;
		size_t count;
		printf(" ucusun Tarihi:");
		cin >> tarih;
		printf(" ucusun Kuyruk No'su:");
		cin >> Kno;
		pstmt = con->prepareStatement("select * from yolcular group by Tckn having Tckn IN(select Tckn from yolcu where Tarih = " + tarih + " AND Kno= '" + Kno + "' GROUP BY Tckn)");
		res = pstmt->executeQuery();
		count = res->rowsCount();//Satýr sayýmýzý sayar döngü için
		cout << "TC No:\t\t\tYolcu Adi\t\tAdres\t\t\tSehir\t\t\tEmail\n";
		for (int i = 0; i <count; i++) {
			res->next();
			cout << res->getString("Tckn") << "\t\t" << res->getString("Yolcu_Adi") << "\t\t" <<  res->getString("Adres") << "\t" << res->getString("Sehir") << "\t\t" << res->getString("Email") << endl;
		}
	}
	catch (sql::SQLException &e) {
		cout << "# Lutfen gecerli veri girisi yapin" ;
	}
}
//En dolu seferi gösterir.
void istatistik() {
	size_t count;
	cout << "\t\t\t\t\t\tISTATISTIK EKRANI\n\n\n";
	pstmt = con->prepareStatement("Select distinct Tarih,Kno,Baslangic,Bitis ,count(*) sayi from yolcu group by Kno,Tarih order by sayi desc");
	res = pstmt->executeQuery();
	count = res->rowsCount();//Satýr sayýmýzý sayar döngü için
	cout << "Tarih:\t\t\tKno\t\tBaslangic\tBitis\t Bilet Sayisi \n";
	for (int i = 0; i <count; i++) {
		res->next();
		cout << res->getString("Tarih") << "\t\t" << res->getString("Kno") << "\t\t" << res->getString("Baslangic") << "\t\t" << res->getString("Bitis") << "\t" << res->getString("sayi") << endl;
	}
}
//Arayüzümüz.
void arayüz() {
	int secim;
	printf("\t\t\t\t\tYey Ulusal Havaalani Otomasyonuna Hosgeldiniz\n\n\n");
	printf("1-Yeni Yolcu Tanimlama\t");
	printf("2-Yeni Ucak Tanimlama\t");
	printf("3-Yeni Sefer Tanimlama\n");
	printf("4-Yolcu Listesi\t");
	printf("\t5-Ucak Listesi\t");
	printf("\t6-Sefer Listesi\n");
	printf("7-Bilet Satma\t");
	printf("\t8-Yolcu Sorgulama\t");
	printf("9-Ucus Iptali\n");
	printf("10-Bilet Iptali\t");
	printf("\t11-Sefer Sorgulama\t");
	printf("12-Istatistik(Doluluk Orani En Fazla Olan Ucus)\n");
	printf("Lutfen yapmak istediginiz islemi seciniz:");
	scanf("%d", &secim);
	switch (secim) {
	case 1:
		system("cls");
		Yeni_Yolcu();
		Menuye_Don();
		break;
	case 2:
		system("cls");
		Yeni_Ucak();
		Menuye_Don();
		break;
	case 3:
		system("cls");
		Yeni_Sefer();
		Menuye_Don();
		break;
	case 4:
		system("cls");
		yolcu_listesi();
		Menuye_Don();
		break;
	case 5:
		system("cls");
		ucak_listesi();
		Menuye_Don();
		break;
	case 6:
		system("cls");
		sefer_listesi();
		Menuye_Don();
		break;
	case 7:
		system("cls");
		bilet_satma();
		Menuye_Don();
		break;
	case 8:
		system("cls");
		yolcu_sorgula();
		Menuye_Don();
		break;
	case 9:
		system("cls");
		ucus_iptal();
		Menuye_Don();
		break;
	case 10:
		system("cls");
		Bilet_Iptal();
		Menuye_Don();
		break;
	case 11:
		system("cls");
		Sefer_Sorgula();
		Menuye_Don();
		break;
	case 12:
		system("cls");
		istatistik();
		Menuye_Don();
		break;
	}
}
int main(void)
{
	database_connection();
	Tablo_Yaratma();
	arayüz();
}