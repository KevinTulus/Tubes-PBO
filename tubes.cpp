#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <ctime>
#include "sha256.cpp"
using namespace std;

int idNow;

class grafik
{
private:
    /* data */
protected:
    void menuKategori()
    {
        cout << "Silahkan Pilih Kategori Dibawah : \n";
        cout << "1. Mata\n";
        cout << "2. THT\n";
        cout << "3. Kulit\n";
        cout << "4. Jantung\n";
        cout << "5. Gigi\n";
        cout << "6. Penyakit Dalam\n";
    }

    void menuUtamaUser()
    {
        cout << "1. Pengajuan\n";
        cout << "2. Cek Status\n";
        cout << "3. Tagihan\n";
        cout << "4. Profile\n";
        cout << "5. Keluar\n";
        cout << "Silahkan Pilih Operasi Yang Diinginkan : ";
    }

    void menuUtamaAdmin()
    {
        cout << "1. Cek User\n";
        cout << "2. Cek Pengajuan User\n";
        cout << "3. Cek User yang Dirawat\n";
        cout << "4. Cek User yang Telah Sembuh\n";
        cout << "5. Cek Berdasarkan Kategori\n";
        cout << "6. Cek Tagihan\n";
        cout << "7. Keluar\n";
        cout << "Silahkan Pilih Operasi Yang Diinginkan : ";
    }

    void menuCekUser()
    {
        cout << "1. Riwayat User\n";
        cout << "2. Update User\n";
        cout << "3. Hapus User\n";
        cout << "4. keluar\n";;
        cout << "Silahkan Pilih Operasi Yang Diinginkan : ";
    }
};

class akun
{
private:
    struct dataAkun
    {
        int idAkun;
        string nik;
        string nama;
        string username;
        string password;
        int level;
        string buatAkun;
    };
    vector <dataAkun> vecAkun;
protected:
    akun()
    {
        startAkun();
    }

    void startAkun()
    {   
        ifstream inAkun("akun.csv");
        string temp;
        getline(inAkun, temp, '\n');
        while(getline(inAkun, temp, '\n'))
        {
            stringstream ss(temp);
            dataAkun tempAkun;
            string idAkunStr;
            getline(ss, idAkunStr, ',');
            tempAkun.idAkun = stoi(idAkunStr);
            getline(ss, tempAkun.nik, ',');
            getline(ss, tempAkun.nama, ',');
            getline(ss, tempAkun.username, ',');
            getline(ss, tempAkun.password, ',');
            string levelStr;
            getline(ss, levelStr, ',');
            tempAkun.level = stoi(levelStr);
            ss >> tempAkun.buatAkun;
            vecAkun.push_back(tempAkun);
        }
        inAkun.close();
    }

    void restartAkun()
    {
        vecAkun.clear();
        startAkun();
    }

    int login(string x, string y)
    {
        SHA256 sha256;
        string hash = sha256(y);
        int cek=0;
        for(auto it = vecAkun.begin(); it != vecAkun.end(); it++)
        {
            if (it->username == x && it->password == hash)
            {
                if (it->level == 1)
                {
                    cek = 1;
                    idNow = it->idAkun;
                    return 1;
                } else
                {
                    cek = 1;
                    idNow = it->idAkun;
                    return 2;
                }
            }
        }
        return 0;
    }

    void registrasi(string nik, string nama, string username, string password)
    {
        int id = vecAkun.size() + 1;
        time_t now = time(0);
        tm* ltm = localtime(&now);
        ofstream out("akun.csv", ios::app);
        out <<endl << id << "," << nik << "," << nama << "," << username << "," << password << ",2," 
            << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << " " << ltm->tm_mday << "-" << 1 + ltm->tm_mon << "-" << 1900 + ltm->tm_year;
        out.close();
        restartAkun();
    }

    void cekProfil()
    {
        for(auto it = vecAkun.begin(); it != vecAkun.end(); it++)
        {
            if (it->idAkun == idNow)
            {
                cout << "NIK : " << it->nik << endl
                    << "Nama : " << it->nama << endl
                    << "Username : " << it->username << endl
                    << "Password : " << it->password << endl;
                cout << endl;
            }
        }
    }

    void gantiProfil(string user, string lama, string baru)
    {
        SHA256 sha256;
        string hashLama = sha256(lama);
        string hashBaru = sha256(baru);
        for(auto it = vecAkun.begin(); it != vecAkun.end(); it++)
        {
            if (it->idAkun == idNow && it->password == hashLama)
            {
                ofstream out1("akun.csv");
                out1 << "idAkun,nik,nama,username,password,buatAkun";
                out1.close();
                ofstream out2("akun.csv", ios::app);
                for(auto it = vecAkun.begin(); it != vecAkun.end(); it++)
                {
                    if (it->idAkun != idNow)
                    {
                        out2 <<endl << it->idAkun << "," << it->nik << "," << it->nama << "," << it->username << "," << it->password << "," << it->level << "," << it->buatAkun;
                    } else if (it->idAkun == idNow)
                    {
                        out2 <<endl << it->idAkun << "," << it->nik << "," << it->nama << "," << user << "," << hashBaru << "," << it->level << "," << it->buatAkun;
                    }
                }
                out2.close();
                restartAkun();
                cout << "Profil Telah Diubah" <<endl;
            } else if (it->idAkun == idNow && it->password != lama)
            {
                cout << "password salah" << endl;
            }
        }   
    }

    void allAkun()
    {
        int cek=0, nomor=1;
        for(auto it = vecAkun.begin(); it != vecAkun.end(); it++)
        {
            if (it->level != 1)
            {
                cout << nomor << ". Id Akun : " << it->idAkun << endl
                    << "NIK : " << it->nik << endl
                    << "Nama : " << it->nama << endl
                    << "Username\t : " << it->username << endl
                    << "Tanggal Dibuat\t : " << it->buatAkun << endl;
                cout << endl;
                nomor++;
                cek = 1;
            }
        }
        if (cek == 0)
        {
            cout << "Anda tidak memiliki keluhan" << endl << endl;
        }
    }

    int cekIdUser(int id)
    {
        int cek=0;
        for(auto it = vecAkun.begin(); it != vecAkun.end(); it++)
        {
            if (it->idAkun == id && it->level ==2)
            {
                cek = 1;
                return 1;
            }
        }
        return 0;
    }

    void updateUser(int id, int nik, string nama, string user)
    {
        for(auto it = vecAkun.begin(); it != vecAkun.end(); it++)
        {
            if (it->idAkun == id)
            {
                ofstream out1("akun.csv");
                out1 << "idAkun,nik,nama,username,password,level,buatAkun";
                out1.close();
                ofstream out2("akun.csv", ios::app);
                for(auto it = vecAkun.begin(); it != vecAkun.end(); it++)
                {
                    if (it->idAkun != idNow)
                    {
                        out2 <<endl << it->idAkun << "," << it->nik << "," << it->nama << "," << it->username<< ","  << it->password << "," << it->level << "," << it->buatAkun;
                    } else if (it->idAkun == idNow)
                    {
                        out2 <<endl << it->idAkun << "," << nik << "," << nama << "," << user << "," << it->password << "," << it->level << ","  << it->buatAkun;
                    }
                }
                out2.close();
                restartAkun();
                cout << "Profil Telah Diubah" <<endl;
            }
        }
    }

    void hapusUser(int id)
    {
        int cek=0;
        for(auto it = vecAkun.begin(); it != vecAkun.end(); it++)
        {
            if (it->idAkun == id && it->level == 2)
            {
                ofstream out1("akun.csv");
                out1 << "idAkun,nik,nama,username,password,level,buatAkun";
                out1.close();
                ofstream out2("akun.csv", ios::app);
                for(auto it = vecAkun.begin(); it != vecAkun.end(); it++)
                {
                    if (it->idAkun != idNow)
                    {
                        out2 <<endl << it->idAkun << "," << it->nik << "," << it->nama << "," << it->username<< "," << it->password << "," << it->level << "," << it->buatAkun;
                    }
                }
                out2.close();
                restartAkun();
                cout << "Profil Telah Diubah" <<endl;
                cek = 1;
            }
            if (cek == 0)
            {
                cout << "User Tidak Ditemukan" << endl;
            }
        }
    }
};

class pasien
{
private:
    struct dataPasien
    {
        int idPasien;
        int idAkun;
        string kategori;
        string keluhan;
        string statusPasien;
        string tglPengajuan;
        string tglCheckIn;
        string tglCheckOut;
        int tagihan;
        string statusTagihan;
    };
    vector <dataPasien> vecPasien;
protected:
    pasien()
    {
        startPasien();
    }

    void startPasien()
    {   
        ifstream inPasien("pasien.csv");
        string temp;
        getline(inPasien, temp, '\n');
        while(getline(inPasien, temp, '\n'))
        {
            stringstream ss(temp);
            dataPasien tempPasien;
            string idPasienStr;
            getline(ss, idPasienStr, ',');
            tempPasien.idPasien = stoi(idPasienStr);
            string idAkunStr;
            getline(ss, idAkunStr, ',');
            tempPasien.idAkun = stoi(idAkunStr);
            getline(ss, tempPasien.kategori, ',');
            getline(ss, tempPasien.keluhan, ',');
            getline(ss, tempPasien.statusPasien, ',');
            getline(ss, tempPasien.tglPengajuan, ',');
            getline(ss, tempPasien.tglCheckIn, ',');
            getline(ss, tempPasien.tglCheckOut, ',');
            ss >> tempPasien.statusTagihan;
            vecPasien.push_back(tempPasien);
        }
        inPasien.close();
    }

    void restartPasien()
    {
        vecPasien.clear();
        startPasien();
    }
    
    void tambah(string kategori, string keluhan)
    {
        int idPasien = vecPasien.size() + 1;
        time_t now = time(0);
        tm* ltm = localtime(&now);
        int idAkun = idNow;
        ofstream out("pasien.csv", ios::app);
        out <<endl << idPasien << "," << idAkun << "," << kategori << "," << keluhan << ",pending," 
            << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << " " << ltm->tm_mday << "-" << 1 + ltm->tm_mon << "-" << 1900 + ltm->tm_year 
            << ",null,null,null,null";
        out.close();
        restartPasien();
    }

    void status()
    {
        int cek=0, nomor=1;
        for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
        {
            if (it->idAkun == idNow)
            {
                cek = 1;
                cout << nomor << ". Kategori : " << it->kategori << endl
                    << "Keluhan : " << it->keluhan << endl
                    << "Tanggal Pengajuan : " << it->tglPengajuan << endl
                    << "Status\t : " << it->statusPasien << endl;
                cout << endl;
                nomor++;
            }
        }
        if (cek == 0)
        {
            cout << "Anda tidak memiliki keluhan" << endl << endl;
        }
    }

    void tagihan()
    {
        int cek=0, nomor=1;
        for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
        {
            if (it->idAkun == idNow && it->statusPasien=="has recovered")
            {
                cek = 1;
                cout << nomor << ". " << it->tagihan << "\t" << it->statusTagihan << endl;
                cout << endl;
                nomor++;
            }
        }
        if (cek == 0)
        {
            cout << "Anda tidak memiliki tagihan" << endl << endl;
        }
    }

    void allPengajuan()
    {
        int cek=0, nomor=1;
        for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
        {
            if (it->statusPasien =="pending")
            {
                cout << nomor << ". Id Pasien : " << it->idPasien << endl
                    << "Id Akun : " << it->idAkun << endl
                    << "Kategori : " << it->kategori << endl
                    << "Keluhan\t : " << it->keluhan << endl
                    << "Tanggal Pengajuan\t : " << it->tglPengajuan << endl;
                cout << endl;
                nomor++;
                cek = 1;
            }
        }
        if (cek == 0)
        {
            cout << "Anda tidak memiliki keluhan" << endl << endl;
        }
    }
    
    int cekIdPasien(int id)
    {
        int cek=0;
        for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
        {
            if (it->idPasien == id)
            {
                cek = 1;
                return 1;
            }
        }
        return 0;
    }

    void terimaPengajuan(int id)
    {
        for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
        {
            if (it->idAkun == id)
            {
                ofstream out1("pasien.csv");
                out1 << "idAkun,nik,nama,username,password,level,buatAkun";
                out1.close();
                ofstream out2("pasien.csv", ios::app);
                time_t now = time(0);
                tm* ltm = localtime(&now);
                for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
                {
                    if (it->idAkun == id)
                    {
                        out2 <<endl << it->idPasien << "," << it->idAkun << "," << it->kategori << "," << it->keluhan << ",treated," << it->tglPengajuan << "," 
                            << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << " " << ltm->tm_mday << "-" << 1 + ltm->tm_mon << "-" << 1900 + ltm->tm_year
                            << ",null,null,null";
                    }
                    if (it->idAkun != id)
                    {
                        out2 <<endl << it->idPasien << "," << it->idAkun << "," << it->kategori << "," << it->keluhan << "," << it->statusPasien << "," << it->tglPengajuan << ",null,null,null,null";
                    }
                }
                out2.close();
                restartPasien();
                cout << "Data Telah Diubah" <<endl;
            }
        }
    }

    void tolakPengajuan(int id)
    {
        for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
        {
            if (it->idAkun == id)
            {
                ofstream out1("pasien.csv");
                out1 << "idAkun,nik,nama,username,password,level,buatAkun";
                out1.close();
                ofstream out2("pasien.csv", ios::app);
                for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
                {
                    if (it->idAkun == id)
                    {
                        out2 <<endl << it->idPasien << "," << it->idAkun << "," << it->kategori << "," << it->keluhan << ",rejected," << it->tglPengajuan << ",null,null,null,null";
                    }
                    if (it->idAkun != id)
                    {
                        out2 <<endl  << it->idPasien << "," << it->idAkun << "," << it->kategori << "," << it->keluhan << "," << it->statusPasien << "," << it->tglPengajuan << ",null,null,null,null";
                    }
                }
                out2.close();
                restartPasien();
                cout << "Data Telah Diubah" <<endl;
            }
        }
    }

    void allDirawat()
    {
        int cek=0, nomor=1;
        for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
        {
            if (it->statusPasien=="treated")
            {
                cout << nomor << ". Id Pasien : " << it->idPasien << endl
                    << "Id Akun : " << it->idAkun << endl
                    << "Kategori : " << it->kategori << endl
                    << "Keluhan\t : " << it->keluhan << endl
                    << "Tanggal Pengajuan\t : " << it->tglPengajuan << endl
                    << "Tanggal Checkin\t : " << it->tglCheckIn << endl;
                cout << endl;
                nomor++;
                cek = 1;
            }
        }
        if (cek == 0)
        {
            cout << "Anda tidak memiliki Pasien" << endl << endl;
        }
    }

    void checkOutUser(int id, int tagihan)
    {
        for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
        {
            if (it->idAkun == id)
            {
                ofstream out1("pasien.csv");
                out1 << "idAkun,nik,nama,username,password,level,buatAkun";
                out1.close();
                ofstream out2("pasien.csv", ios::app);
                time_t now = time(0);
                tm* ltm = localtime(&now);
                for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
                {
                    if (it->idAkun == id)
                    {
                        out2 <<endl << "," << it->idPasien << "," << it->idAkun << "," << it->kategori << "," << it->keluhan << ",has checkout," << it->tglPengajuan << "," << it->tglCheckIn << "," 
                            << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << " " << ltm->tm_mday << "-" << 1 + ltm->tm_mon << "-" << 1900 + ltm->tm_year
                            << "," << tagihan << ",not paid yet";
                    }
                    if (it->idAkun != id)
                    {
                        out2 <<endl << "," << it->idPasien << "," << it->idAkun << "," << it->kategori << "," << it->keluhan << "," << it->statusPasien << "," << it->tglPengajuan << "," << it->tglCheckIn << ",null,null,null";
                    }
                }
                out2.close();
                restartPasien();
                cout << "Data Telah Diubah" <<endl;
            }
        }
    }

    void allSembuh()
    {
        int cek=0, nomor=1;
        for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
        {
            if (it->statusPasien=="has checkout")
            {
                cout << nomor << ". Id Pasien : " << it->idPasien << endl
                    << "Id Akun : " << it->idAkun << endl
                    << "Kategori : " << it->kategori << endl
                    << "Keluhan\t : " << it->keluhan << endl
                    << "Tanggal Pengajuan\t : " << it->tglPengajuan << endl
                    << "Tanggal Checkin\t : " << it->tglCheckIn << endl
                    << "Tanggal Checkout\t : " << it->tglCheckOut << endl;
                cout << endl;
                nomor++;
                cek = 1;
            }
        }
        if (cek == 0)
        {
            cout << "Anda tidak memiliki Pasien" << endl << endl;
        }
    }

    void allTagihan()
    {
        int cek=0, nomor=1;
        for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
        {
            if (it->statusPasien=="has checkout" && it->statusTagihan=="not paid yet")
            {
                cout << nomor << ". Id Pasien : " << it->idPasien << endl
                    << "Id Akun : " << it->idAkun << endl
                    << "Tagihan : " << it->tagihan << endl;
                cout << endl;
                nomor++;
                cek = 1;
            }
        }
        if (cek == 0)
        {
            cout << "Anda tidak memiliki Pasien" << endl << endl;
        }
    }

    void bayarTagihan(int id)
    {
        for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
        {
            if (it->idPasien == id)
            {
                ofstream out1("pasien.csv");
                out1 << "idAkun,nik,nama,username,password,level,buatAkun";
                out1.close();
                ofstream out2("pasien.csv", ios::app);
                for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
                {
                    if (it->idPasien == id)
                    {
                        out2 <<endl << "," << it->idPasien << "," << it->idAkun << "," << it->kategori << "," << it->keluhan << "," << it->statusPasien << "," << it->tglPengajuan << "," << it->tglCheckIn << "," << it->tglCheckOut << "," << it->tagihan << ",paid";
                    }
                    if (it->idPasien != id)
                    {
                        out2 <<endl << "," << it->idPasien << "," << it->idAkun << "," << it->kategori << "," << it->keluhan << "," << it->statusPasien << "," << it->tglPengajuan << "," << it->tglCheckIn << "," << it->tglCheckOut << "," << it->tagihan << "," << it->statusTagihan;
                    }
                }
                out2.close();
                restartPasien();
                cout << "Data Telah Diubah" <<endl;
            }
        }
    }

    void riwayatUser(int id)
    {
        int cek=0, nomor=1;
        for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
        {
            if (it->idAkun == id)
            {
                cout << nomor << ". Id Pasien : " << it->idPasien << endl
                    << "Kategori : " << it->kategori << endl
                    << "Keluhan\t : " << it->keluhan << endl
                    << "Tanggal Pengajuan\t : " << it->tglPengajuan << endl
                    << "Tanggal Checkin\t : " << it->tglCheckIn << endl
                    << "Tanggal Checkout\t : " << it->tglCheckOut << endl
                    << "Tagihan\t : " << it->tagihan << endl
                    << "Status Tagihan\t : " << it->statusTagihan << endl;
                cout << endl;
                nomor++;
                cek = 1;
            }
        }
        if (cek == 0)
        {
            cout << "Anda tidak memiliki keluhan" << endl << endl;
        }
    }

    void kategori(string type)
    {
        int cek=0, nomor=1;
        for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
        {
            if (it->kategori == type)
            {
                cout << nomor << ". Id Pasien : " << it->idPasien << endl
                    << "Kategori : " << it->kategori << endl
                    << "Keluhan\t : " << it->keluhan << endl
                    << "Tanggal Pengajuan\t : " << it->tglPengajuan << endl
                    << "Tanggal Checkin\t : " << it->tglCheckIn << endl
                    << "Tanggal Checkout\t : " << it->tglCheckOut << endl
                    << "Tagihan\t : " << it->tagihan << endl
                    << "Status Tagihan\t : " << it->statusTagihan << endl;
                cout << endl;
                nomor++;
                cek = 1;
            }
        }
        if (cek == 0)
        {
            cout << "Anda tidak memiliki Pasien" << endl << endl;
        }
    }
};

class user: protected grafik, protected pasien, protected akun
{
private:
    /* data */
protected:
    void utamaUser()
    {
        int x=1;
        char pilih;
        do
        {
            system("cls");
            menuUtamaUser();
            cin >> pilih;
            cout <<endl;
            system("cls");
            
            switch (pilih)
            {
            case '1':
                pengajuan();
                break;
            
            case '2':
                status();
                break;
            
            case '3':
                tagihan();
                break;
            
            case '4':
                profile();
                break;
            
            case '5':
                x=0;
                break;
            
            default:
                cout << "Input Salah\n\n";
                break;
            }
        } while (x==1);
    }

    void pengajuan()
    {
        int type,x=1;
        char pilih;
        string isi, kategori;
        menuKategori();
        cin >> type;
        
        switch (type)
        {
        case '1':
            kategori = "Mata";
            break;
        
        case '2':
            kategori = "THT";
            break;
        
        case '3':
            kategori = "Kulit";
            break;

        case '4':
            kategori = "Jantung";
            break;

        case '5':
            kategori = "Gigi";
            break;

        case '6':
            kategori = "Penyakit Dalam";
            break;
        
        default:
            break;
        }
        system("cls");
        cout << kategori << endl;
        cout << "Keluhan\n";
        cin.ignore();
        getline(cin, isi);
        do
        {
            cout << "Apakah anda mengkonfirmasi Keluhan?\n";
            cout << "1. Iya\n";
            cout << "2. Tidak\n";
            cin >> pilih;
            cout <<endl;
            system("cls");
            
            switch (pilih)
            {
            case '1':
                tambah(kategori, isi);
                x=0;
                break;
            
            case '2':
                x=0;
                break;
            
            default:
                cout << "Input Salah\n\n";
                break;
            }
        } while (x==1);
    }

    void profile()
    {
        cekProfil();
        int x=1;
        char pilih;
        string user, lama, baru;
        do
        {
            cout << "Apakah anda ingin mengganti username dan password?\n";
            cout << "1. Iya\n";
            cout << "2. Tidak\n";
            cin >> pilih;
            cout <<endl;
            system("cls");
            
            switch (pilih)
            {
            case '1':
                cout << "Masukan Username Baru : ";
                cin >> user;
                cout << "Masukan Password Lama : ";
                cin >> lama;
                cout << "Masukan Password Baru : ";
                cin >> baru;
                gantiProfil(user, lama, baru);
                x=0;
                break;
            
            case '2':
                x=0;
                break;
            
            default:
                cout << "Input Salah\n\n";
                break;
            }
        } while (x==1);
    }
};

class admin: protected user
{
private:
    /* data */
protected:
    void utamaAdmin()
    {
        int x=1;
        char pilih;
        do
        {
            system("cls");
            cout << "Keluhan\n";
            menuUtamaAdmin();
            cin >> pilih;
            cout <<endl;
            
            switch (pilih)
            {
            case '1':
                cekUser();
                break;

            case '2':
                cekPengajuan();
                break;
            
            case '3':
                cekDirawat();
                break;

            case '4':
                allSembuh();
                break;
            
            case '5':
                cekKategori();
                break;

            case '6':
                cekTagihan();
                break;
            
            case '7':
                x=0;
                break;
            
            default:
                cout << "Input Salah\n\n";
                break;
            }
        } while (x==1);
    }

    void cekUser()
    {
        int x=1, id, nik;
        char pilih;
        string nama, user, pass;
        do
        {
            allAkun();
            menuCekUser();
            cin >> pilih;
            cout <<endl;
            
            switch (pilih)
            {
            case '1':
                cout << "Masukan Id User : ";
                cin >> id;
                cout <<endl;
                riwayatUser(id);
                break;
            
            case '2':
                cout << "Masukan Id User : ";
                cin >> id;
                cout <<endl;
                if (cekIdUser(id) == 1)
                {
                    cout << "Masukan NIK : ";
                    cin >> nik;
                    cout << "Masukan Nama Lengkap : ";
                    cin.ignore();
                    getline(cin, nama);
                    cout << "Masukan Username : ";
                    cin >> user;
                    cout << "Masukan Password : ";
                    cin >> pass;
                    cout << endl;
                    updateUser(id, nik, nama, user);
                } else 
                {
                    cout << "User tidak ditemukan \n";
                }
                break;

            case '3':
                cout << "Masukan Id User : ";
                cin >> id;
                cout << endl;
                hapusUser(id);
                break;

            case '4':
                x=0;
                break;
            
            default:
                cout << "Input Salah\n\n";
                break;
            }
        } while (x==1);
    }
    
    void cekPengajuan()
    {
        int x=1, id;
        char pilih;
        do
        {
            allPengajuan();
            cout << "1. Terima Keluhan\n";
            cout << "2. Tolak Keluhan\n";
            cout << "3. Kembali\n";
            cout << "Silahkan Pilih Operasi Yang Diinginkan : ";
            cin >> pilih;
            cout <<endl;
            
            switch (pilih)
            {
            case '1':
                cout << "Masukan Id User : ";
                cin >> id;
                cout <<endl;
                if (cekIdUser(id) == 1)
                {
                    terimaPengajuan(id);
                } else 
                {
                    cout << "User tidak ditemukan \n";
                }
                break;
            
            case '2':
                cout << "Masukan Id User : ";
                cin >> id;
                cout <<endl;
                if (cekIdUser(id) == 1)
                {
                    tolakPengajuan(id);
                } else 
                {
                    cout << "User tidak ditemukan \n";
                }
                break;

            case '3':
                x=0;
                break;
            
            default:
                cout << "Input Salah\n\n";
                break;
            }
        } while (x==1);
    }

    void cekDirawat()
    {
        int x=1, id, tagihan;
        char pilih;
        do
        {
            allDirawat();
            cout << "1. Checkout User\n";
            cout << "2. Kembali\n";
            cout << "Silahkan Pilih Operasi Yang Diinginkan : ";
            cin >> pilih;
            cout <<endl;
            
            switch (pilih)
            {
            case '1':
                cout << "Masukan Id User : ";
                cin >> id;
                cout <<endl;
                if (cekIdUser(id) == 1)
                {
                    cout << "Masukan Tagihan User : ";
                    cin >> tagihan;
                    checkOutUser(id, tagihan);
                } else 
                {
                    cout << "User tidak ditemukan \n";
                }
                break;
            
            case '2':
                x=0;
                break;
            
            default:
                cout << "Input Salah\n\n";
                break;
            }
        } while (x==1);
    }

    void cekKategori()
    {
        int x=1, id;
        char pilih;
        string jenis;
        do
        {
            menuKategori();
            cin >> pilih;
            if (pilih == 1)
            {
                jenis = "Mata";
            } else if (pilih == 2)
            {
                jenis = "THT";
            } else if (pilih == 3)
            {
                jenis = "Kulit";
            } else if (pilih == 4)
            {
                jenis = "Jantung";
            } else if (pilih == 5)
            {
                jenis = "Gigi";
            } else if (pilih == 6)
            {
                jenis = "Penyakit Dalam";
            }
            cout <<endl;
            if (pilih <= 6)
            {
                kategori(jenis);
            } else if (pilih = 7)
            {
                x = 0;
            }
        } while (x==1);

        
    }

    void cekTagihan()
    {
        int x=1, id;
        char pilih;
        do
        {
            allTagihan();
            cout << "1. Bayar Tagihan User\n";
            cout << "2. Kembali\n";
            cout << "Silahkan Pilih Operasi Yang Diinginkan : ";
            cin >> pilih;
            cout <<endl;
            
            switch (pilih)
            {
            case '1':
                cout << "Masukan Id Pasien : ";
                cin >> id;
                if (cekIdPasien(id) == 1)
                {
                    bayarTagihan(id);
                } else 
                {
                    cout << "User tidak ditemukan \n";
                }
                break;
            
            case '2':
                x=0;
                break;
            
            default:
                cout << "Input Salah\n\n";
                break;
            }
        } while (x==1);
    }
};

class auth: protected admin
{
private:
    /* data */
protected:
    void menuLogin()
    {
        string user,pass;
        cout << "Masukan Username : ";
        cin >> user;
        cout << "Masukan Password : ";
        cin >> pass;
        cout << endl;
        if (akun::login(user, pass) == 2)
        {
            utamaUser();
        } else if (akun::login(user, pass) == 1)
        {
            utamaAdmin();
        }
        else
        {
            cout << "Username dan Password salah\n";
        }

    }

    void menuRegistrasi()
    {
        SHA256 sha256;
        string nik, nama, user, pass, hash;
        cout << "Masukan NIK : ";
        cin >> nik;
        cout << "Masukan Nama Lengkap : ";
        cin.ignore();
        getline(cin, nama);
        cout << "Masukan Username : ";
        cin >> user;
        cout << "Masukan Password : ";
        cin >> pass;
        hash = sha256(pass);
        cout << endl;
        akun::registrasi(nik, nama, user, hash);
    }
};


class init: private auth
{
private:
    /* data */
public:
    void loginRegistrasi()
    {
        int x=1;
        char pilih;
        do
        {
            cout << "1. Login\n";
            cout << "2. Registrasi\n";
            cout << "Silahkan Pilih Operasi Yang Diinginkan : ";
            cin >> pilih;
            cout <<endl;
            system("cls");
            
            switch (pilih)
            {
            case '1':
                menuLogin();
                break;
            
            case '2':
                menuRegistrasi();
                break;
            
            default:
                cout << "Input Salah\n\n";
                break;
            }
        } while (x==1);
    }
};


int main()
{
    init in;
    in.loginRegistrasi();
    return 0;
}