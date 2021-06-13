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
        cout << "Silahkan pilih kategori dibawah : \n";
        cout << "1. Mata\n";
        cout << "2. THT\n";
        cout << "3. Kulit\n";
        cout << "4. Jantung\n";
        cout << "5. Gigi\n";
        cout << "6. Penyakit dalam\n";
    }

    void menuUtamaUser()
    {
        cout << "1. Pengajuan\n";
        cout << "2. Cek status\n";
        cout << "3. Tagihan\n";
        cout << "4. Profile\n";
        cout << "5. Keluar\n";
        cout << "Silahkan pilih operasi yang diinginkan : ";
    }

    void menuUtamaAdmin()
    {
        cout << "1. Cek user\n";
        cout << "2. Cek pengajuan user\n";
        cout << "3. Cek user yang dirawat\n";
        cout << "4. Cek user yang telah sembuh\n";
        cout << "5. Cek berdasarkan kategori\n";
        cout << "6. Cek tagihan\n";
        cout << "7. Keluar\n";
        cout << "Silahkan pilih operasi yang diinginkan : ";
    }

    void menuCekUser()
    {
        cout << "1. Riwayat user\n";
        cout << "2. Update user\n";
        cout << "3. Hapus user\n";
        cout << "4. Keluar\n";;
        cout << "Silahkan pilih operasi yang diinginkan : ";
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
        cekAdmin();
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
            getline(ss, tempAkun.buatAkun, ',');
            vecAkun.push_back(tempAkun);
        }
        inAkun.close();
    }

    void cekAdmin()
    {
        SHA256 sha256;
        string password = sha256("pass");
        int cek = 0;
        for(auto it = vecAkun.begin(); it != vecAkun.end(); it++)
        {
            if (it->idAkun == 0)
            {
                cek++;
            }
        }
        if (cek == 0)
        {
            time_t now = time(0);
            tm* ltm = localtime(&now);
            ofstream out("akun.csv", ios::app);
            out <<endl << "0,0,admin,admin," << password << ",1," 
                << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << " " << ltm->tm_mday << "-" << 1 + ltm->tm_mon << "-" << 1900 + ltm->tm_year;
            out.close();
        }
        restartAkun();
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
                cout << "NIK \t\t: " << it->nik << endl
                    << "Nama \t\t: " << it->nama << endl
                    << "Username \t: " << it->username << endl
                    << "Password \t: " << it->password << endl;
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
                system("cls");
                cout << "Profil telah diubah" <<endl<<endl;
            } else if (it->idAkun == idNow && it->password != lama)
            {
                system("cls");
                cout << "Password salah" << endl<<endl;
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
                cout << nomor << ". Id akun \t\t: " << it->idAkun << endl
                    << "   NIK \t\t\t: " << it->nik << endl
                    << "   Nama \t\t: " << it->nama << endl
                    << "   Username \t\t: " << it->username << endl
                    << "   Tanggal dibuat \t: " << it->buatAkun << endl;
                cout << endl;
                nomor++;
                cek = 1;
            }
        }
        if (cek == 0)
        {
            cout << "Tidak terdapat user" << endl << endl;
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
                    if (it->idAkun != id)
                    {
                        out2 <<endl << it->idAkun << "," << it->nik << "," << it->nama << "," << it->username<< ","  << it->password << "," << it->level << "," << it->buatAkun;
                    } else if (it->idAkun == id)
                    {
                        out2 <<endl << it->idAkun << "," << nik << "," << nama << "," << user << "," << it->password << "," << it->level << "," << it->buatAkun;
                    }
                }
                out2.close();
                restartAkun();
                system("cls");
                cout << "Profil telah diubah" <<endl<<endl;
            }
        }
    }

    void hapusUser(int id)
    {
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
                    if (it->idAkun != id)
                    {
                        out2 <<endl << it->idAkun << "," << it->nik << "," << it->nama << "," << it->username<< "," << it->password << "," << it->level << "," << it->buatAkun;
                    }
                }
                out2.close();
                restartAkun();
                system("cls");
                cout << "Profil telah dihapus" <<endl<<endl;
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
            string tagihanStr;
            getline(ss, tagihanStr, ',');
            tempPasien.tagihan = stoi(tagihanStr);
            getline(ss, tempPasien.statusTagihan, ',');
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
            << ",tidak checkin,tidak checkout,0,tidak memiliki tagihan";
        out.close();
        restartPasien();
        system("cls");
        cout << "Keluhan telah ditambahkan" << endl << endl;
    }

    void status()
    {
        int cek=0, nomor=1;
        for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
        {
            if (it->idAkun == idNow)
            {
                cek = 1;
                cout << nomor << ". Kategori \t\t: " << it->kategori << endl
                    << "   Keluhan \t\t: " << it->keluhan << endl
                    << "   Tanggal pengajuan \t: " << it->tglPengajuan << endl
                    << "   Status \t\t: " << it->statusPasien << endl;
                cout << endl;
                nomor++;
            }
        }
        if (cek == 0)
        {
            system("cls");
            cout << "Anda tidak memiliki keluhan" << endl << endl;
        }
    }

    void tagihan()
    {
        int cek=0, nomor=1;
        for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
        {
            if (it->idAkun == idNow && it->statusPasien=="has checkout")
            {
                cek = 1;
                cout << nomor << ". " << it->tagihan << "\t" << it->statusTagihan << endl;
                cout << endl;
                nomor++;
            }
        }
        if (cek == 0)
        {
            system("cls");
            cout << "Anda tidak terdapat tagihan" << endl << endl;
        }
    }

    void allPengajuan()
    {
        int cek=0, nomor=1;
        for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
        {
            if (it->statusPasien =="pending")
            {
                cout << nomor << ". Id pasien : " << it->idPasien << endl
                    << "Id Akun : " << it->idAkun << endl
                    << "Kategori : " << it->kategori << endl
                    << "Keluhan\t : " << it->keluhan << endl
                    << "Tanggal pengajuan\t : " << it->tglPengajuan << endl;
                cout << endl;
                nomor++;
                cek = 1;
            }
        }
        if (cek == 0)
        {
            system("cls");
            cout << "Tidak terdapat keluhan" << endl << endl;
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
            if (it->idPasien == id)
            {
                ofstream out1("pasien.csv");
                out1 << "idPasien,idAkun,kategori,keluhan,statusPasien,tglPengajuan,tglCheckIn,tglCheckOut,tagihan,statusTagihan";
                out1.close();
                ofstream out2("pasien.csv", ios::app);
                time_t now = time(0);
                tm* ltm = localtime(&now);
                for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
                {
                    if (it->idPasien == id)
                    {
                        out2 <<endl << it->idPasien << "," << it->idAkun << "," << it->kategori << "," << it->keluhan << ",treated," << it->tglPengajuan << "," 
                            << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << " " << ltm->tm_mday << "-" << 1 + ltm->tm_mon << "-" << 1900 + ltm->tm_year
                            << "," << it->tglCheckOut << "," << it->tagihan << "," << it->statusTagihan;
                    }
                    if (it->idPasien != id)
                    {
                        out2 <<endl << it->idPasien << "," << it->idAkun << "," << it->kategori << "," << it->keluhan << "," << it->statusPasien 
                            << "," << it->tglPengajuan << "," << it->tglCheckIn << "," << it->tglCheckOut << "," << it->tagihan << "," << it->statusTagihan;
                    }
                }
                out2.close();
                restartPasien();
                system("cls");
                cout << "Pengajuan telah diterima" <<endl<<endl;
            }
        }
    }

    void tolakPengajuan(int id)
    {
        for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
        {
            if (it->idPasien == id)
            {
                ofstream out1("pasien.csv");
                out1 << "idPasien,idAkun,kategori,keluhan,statusPasien,tglPengajuan,tglCheckIn,tglCheckOut,tagihan,statusTagihan";
                out1.close();
                ofstream out2("pasien.csv", ios::app);
                for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
                {
                    if (it->idPasien == id)
                    {
                        out2 <<endl << it->idPasien << "," << it->idAkun << "," << it->kategori << "," << it->keluhan << ",rejected," << it->tglPengajuan 
                            << "," << it->tglCheckIn << "," << it->tglCheckOut << "," << it->tagihan << "," << it->statusTagihan;
                    }
                    if (it->idPasien != id)
                    {
                        out2 <<endl << it->idPasien << "," << it->idAkun << "," << it->kategori << "," << it->keluhan << "," << it->statusPasien 
                            << "," << it->tglPengajuan << "," << it->tglCheckIn << "," << it->tglCheckOut << "," << it->tagihan << "," << it->statusTagihan;
                    }
                }
                out2.close();
                restartPasien();
                system("cls");
                cout << "Pengajuan telah ditolak" <<endl<<endl;
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
                cout << nomor << ". Id Pasien \t\t: " << it->idPasien << endl
                    << "   Id Akun \t\t: " << it->idAkun << endl
                    << "   Kategori \t\t: " << it->kategori << endl
                    << "   Keluhan \t\t: " << it->keluhan << endl
                    << "   Tanggal Pengajuan \t: " << it->tglPengajuan << endl
                    << "   Tanggal Checkin \t: " << it->tglCheckIn << endl;
                cout << endl;
                nomor++;
                cek = 1;
            }
        }
        if (cek == 0)
        {
            system("cls");
            cout << "Tidak terdapat pasien yang sedang dirawat" << endl << endl;
        }
    }

    void checkOutUser(int id, int tagihan)
    {
        for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
        {
            if (it->idPasien == id)
            {
                ofstream out1("pasien.csv");
                out1 << "idPasien,idAkun,kategori,keluhan,statusPasien,tglPengajuan,tglCheckIn,tglCheckOut,tagihan,statusTagihan";
                out1.close();
                ofstream out2("pasien.csv", ios::app);
                time_t now = time(0);
                tm* ltm = localtime(&now);
                for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
                {
                    if (it->idPasien == id)
                    {
                        out2 << endl << it->idPasien << "," << it->idAkun << "," << it->kategori << "," << it->keluhan << ",has checkout," << it->tglPengajuan << "," << it->tglCheckIn << "," 
                            << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << " " << ltm->tm_mday << "-" << 1 + ltm->tm_mon << "-" << 1900 + ltm->tm_year
                            << "," << tagihan << ",not paid yet";
                    }
                    if (it->idPasien != id)
                    {
                        out2 <<endl << it->idPasien << "," << it->idAkun << "," << it->kategori << "," << it->keluhan << "," << it->statusPasien 
                            << "," << it->tglPengajuan << "," << it->tglCheckIn << "," << it->tglCheckOut << "," << it->tagihan << "," << it->statusTagihan;
                    }
                }
                out2.close();
                restartPasien();
                system("cls");
                cout << "Pasien telah checkout" <<endl<<endl;
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
                cout << nomor << ". Id Pasien \t\t: " << it->idPasien << endl
                    << "   Id Akun \t\t: " << it->idAkun << endl
                    << "   Kategori \t\t: " << it->kategori << endl
                    << "   Keluhan \t\t: " << it->keluhan << endl
                    << "   Tanggal Pengajuan \t: " << it->tglPengajuan << endl
                    << "   Tanggal Checkin \t: " << it->tglCheckIn << endl
                    << "   Tanggal Checkout \t: " << it->tglCheckOut << endl;
                cout << endl;
                nomor++;
                cek = 1;
            }
        }
        if (cek == 0)
        {
            system("cls");
            cout << "Tidak terdapat pasien yang sembuh" << endl << endl;
        }
    }

    void allTagihan()
    {
        int cek=0, nomor=1;
        for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
        {
            if (it->statusPasien=="has checkout" && it->statusTagihan=="not paid yet")
            {
                cout << nomor << ". Id Pasien \t: " << it->idPasien << endl
                    << "   Id Akun \t: " << it->idAkun << endl
                    << "   Tagihan \t: " << it->tagihan << endl;
                cout << endl;
                nomor++;
                cek = 1;
            }
        }
        if (cek == 0)
        {
            system("cls");
            cout << "Tidak terdapat pasien yang memiliki tagihan" << endl << endl;
        }
    }

    void bayarTagihan(int id)
    {
        for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
        {
            if (it->idPasien == id)
            {
                ofstream out1("pasien.csv");
                out1 << "idPasien,idAkun,kategori,keluhan,statusPasien,tglPengajuan,tglCheckIn,tglCheckOut,tagihan,statusTagihan";
                out1.close();
                ofstream out2("pasien.csv", ios::app);
                for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
                {
                    if (it->idPasien == id)
                    {
                        out2 <<endl << it->idPasien << "," << it->idAkun << "," << it->kategori << "," << it->keluhan << "," << it->statusPasien 
                            << "," << it->tglPengajuan << "," << it->tglCheckIn << "," << it->tglCheckOut << "," << it->tagihan << ",paid";
                    }
                    if (it->idPasien != id)
                    {
                        out2 <<endl << it->idPasien << "," << it->idAkun << "," << it->kategori << "," << it->keluhan << "," << it->statusPasien 
                            << "," << it->tglPengajuan << "," << it->tglCheckIn << "," << it->tglCheckOut << "," << it->tagihan << "," << it->statusTagihan;
                    }
                }
                out2.close();
                restartPasien();
                system("cls");
                cout << "Tagihan telah diubah" <<endl<<endl;
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
                cout << nomor << ". Id pasien \t\t: " << it->idPasien << endl
                    << "   Kategori \t\t: " << it->kategori << endl
                    << "   Keluhan \t\t: " << it->keluhan << endl
                    << "   Tanggal pengajuan \t: " << it->tglPengajuan << endl
                    << "   Tanggal checkin \t: " << it->tglCheckIn << endl
                    << "   Tanggal checkout \t: " << it->tglCheckOut << endl
                    << "   Tagihan \t\t: " << it->tagihan << endl
                    << "   Status tagihan \t: " << it->statusTagihan << endl;
                cout << endl;
                nomor++;
                cek = 1;
            }
        }
        if (cek == 0)
        {
            system("cls");
            cout << "Tidak terdapat keluhan" << endl << endl;
        }
    }

    void kategori(string type)
    {
        int cek=0, nomor=1;
        for(auto it = vecPasien.begin(); it != vecPasien.end(); it++)
        {
            if (it->kategori == type)
            {
                cout << nomor << ". Id pasien \t\t: " << it->idPasien << endl
                    << "   Kategori \t\t: " << it->kategori << endl
                    << "   Keluhan \t\t: " << it->keluhan << endl
                    << "   Tanggal pengajuan \t: " << it->tglPengajuan << endl
                    << "   Tanggal checkin \t: " << it->tglCheckIn << endl
                    << "   Tanggal checkout \t: " << it->tglCheckOut << endl
                    << "   Tagihan \t\t: " << it->tagihan << endl
                    << "   Status tagihan \t: " << it->statusTagihan << endl;
                cout << endl;
                nomor++;
                cek = 1;
            }
        }
        if (cek == 0)
        {
            system("cls");
            cout << "Tidak tedapat pasien" << endl << endl;
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
                cout << "Input salah\n\n";
                break;
            }
        } while (x==1);
    }

    void pengajuan()
    {
        int x=1, y=1;
        char type, pilih;
        string isi, kategori;
        do
        {
            menuKategori();
            cin >> type;
            switch (type)
            {
            case '1':
                kategori = "Mata";
                y=0;
                break;
            
            case '2':
                kategori = "THT";
                y=0;
                break;
            
            case '3':
                kategori = "Kulit";
                y=0;
                break;

            case '4':
                kategori = "Jantung";
                y=0;
                break;

            case '5':
                kategori = "Gigi";
                y=0;
                break;

            case '6':
                kategori = "Penyakit dalam";
                y=0;
                break;
            
            default:
                cout << "Input salah\n\n";
                break;
            }
        } while (y == 1);
        
        
        system("cls");
        cout << kategori << endl;
        cout << "Keluhan :\n";
        cin.ignore();
        getline(cin, isi);
        do
        {
            cout << "Apakah anda mengkonfirmasi keluhan?\n";
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
                cout << "Input salah\n\n";
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
                cout << "Masukan username baru : ";
                cin >> user;
                cout << "Masukan password lama : ";
                cin >> lama;
                cout << "Masukan password baru : ";
                cin >> baru;
                gantiProfil(user, lama, baru);
                x=0;
                break;
            
            case '2':
                x=0;
                break;
            
            default:
                cout << "Input salah\n\n";
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
            menuUtamaAdmin();
            cin >> pilih;
            cout <<endl;
            system("cls");
            
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
                cout << "Input salah\n\n";
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
                system("cls");
                allAkun();
                cout << "Masukan id akun : ";
                cin >> id;
                cout <<endl;
                riwayatUser(id);
                break;
            
            case '2':
                cout << "Masukan id akun : ";
                cin >> id;
                cout <<endl;
                if (cekIdUser(id) == 1)
                {
                    cout << "Masukan NIK : ";
                    cin >> nik;
                    cout << "Masukan nama lengkap : ";
                    cin.ignore();
                    getline(cin, nama);
                    cout << "Masukan username : ";
                    cin >> user;
                    cout << endl;
                    updateUser(id, nik, nama, user);
                } else 
                {
                    system("cls");
                    cout << "User tidak ditemukan \n\n";
                }
                break;

            case '3':
                cout << "Masukan Id Akun : ";
                cin >> id;
                cout << endl;
                if (cekIdUser(id) == 1)
                {
                    hapusUser(id);
                } else 
                {
                    system("cls");
                    cout << "User tidak ditemukan \n\n";
                }
                break;

            case '4':
                system("cls");
                x=0;
                break;
            
            default:
                cout << "Input salah\n\n";
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
            cout << "1. Terima keluhan\n";
            cout << "2. Tolak keluhan\n";
            cout << "3. Kembali\n";
            cout << "Silahkan pilih operasi yang diinginkan : ";
            cin >> pilih;
            cout <<endl;
            
            switch (pilih)
            {
            case '1':
                cout << "Masukan id pasien : ";
                cin >> id;
                cout <<endl;
                if (cekIdPasien(id) == 1)
                {
                    terimaPengajuan(id);
                } else 
                {
                    system("cls");
                    cout << "User tidak ditemukan \n\n";
                }
                break;
            
            case '2':
                cout << "Masukan id pasien : ";
                cin >> id;
                cout <<endl;
                if (cekIdPasien(id) == 1)
                {
                    tolakPengajuan(id);
                } else 
                {
                    system("cls");
                    cout << "User tidak ditemukan \n\n";
                }
                break;

            case '3':
                system("cls");
                x=0;
                break;
            
            default:
                cout << "Input salah\n\n";
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
            cout << "1. Checkout user\n";
            cout << "2. Kembali\n";
            cout << "Silahkan pilih operasi yang diinginkan : ";
            cin >> pilih;
            cout <<endl;
            
            switch (pilih)
            {
            case '1':
                cout << "Masukan id pasien : ";
                cin >> id;
                cout <<endl;
                if (cekIdPasien(id) == 1)
                {
                    cout << "Masukan tagihan user : ";
                    cin >> tagihan;
                    checkOutUser(id, tagihan);
                } else 
                {
                    system("cls");
                    cout << "User tidak ditemukan \n\n";
                }
                break;
            
            case '2':
                system("cls");
                x=0;
                break;
            
            default:
                cout << "Input salah\n\n";
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
            cout << "7. Keluar\n";
            cin >> pilih;
            switch (pilih)
            {
            case '1':
                system("cls");
                jenis = "Mata";
                kategori(jenis);
                break;
            
            case '2':
                system("cls");
                jenis = "THT";
                kategori(jenis);
                break;
            
            case '3':
                system("cls");
                jenis = "Kulit";
                kategori(jenis);
                break;

            case '4':
                system("cls");
                jenis = "Jantung";
                kategori(jenis);
                break;

            case '5':
                system("cls");
                jenis = "Gigi";
                kategori(jenis);
                break;

            case '6':
                system("cls");
                jenis = "Penyakit dalam";
                kategori(jenis);
                break;

            case '7':
                system("cls");
                x=0;
                break;
            
            default:
                cout << "Input salah\n\n";
                break;
            }
        } while (x == 1);
    }

    void cekTagihan()
    {
        int x=1, id;
        char pilih;
        do
        {
            allTagihan();
            cout << "1. Bayar tagihan user\n";
            cout << "2. Kembali\n";
            cout << "Silahkan pilih operasi yang diinginkan : ";
            cin >> pilih;
            cout <<endl;
            
            switch (pilih)
            {
            case '1':
                cout << "Masukan id pasien : ";
                cin >> id;
                if (cekIdPasien(id) == 1)
                {
                    bayarTagihan(id);
                } else 
                {
                    system("cls");
                    cout << "User tidak ditemukan \n\n";
                }
                break;
            
            case '2':
                system("cls");
                x=0;
                break;
            
            default:
                cout << "Input salah\n\n";
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
            system("cls");
            utamaUser();
        } else if (akun::login(user, pass) == 1)
        {
            system("cls");
            utamaAdmin();
        }
        else
        {
            system("cls");
            cout << "Username dan Password salah\n\n";
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
        system("cls");
        cout << "Akun telah terdaftar\n\n";
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
            cout << "Silahkan pilih operasi yang diinginkan : ";
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
    system("cls");
    init in;
    in.loginRegistrasi();
    return 0;
}