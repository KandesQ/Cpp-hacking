#include <iostream>
#include <thread>

using namespace std;

/*
    Программа-имитация соревнования между компьютерами.

    Пользователь вводит слово и кол-во компьютеров-участников, которые будут подбирать введеное слово.
    Кто первый вернет слово, тот и победил.
*/

// пока winnerName пустая строка - значит что никто еще не подобрал пароль, когда кто то подберет
// он изменит ее на свое имя, размер имени 1 КБ
char winnerName[1024] = "";

// все символы с клавиатуры
char symbols[] = "`1234567890-=qwertyuiop[]\\asdfghjkl;'zxcvbnm,./~!@#$%^&*()_+|}{\":?><№QWERTYUIOPASDFGHJKLZXCVBNMйцукенгшщзхъёфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪЁФЫВАПРОЛДЖЭЯЧСМИТЬБЮ";
int symbolsCount = strlen(symbols);

/*
 *проблемы: непрвильный вывод имен, выводится только один хакер
 */


bool startWith(char* actualPassword, int actualPasswordSize, char* expectedPassword) {
    /*
     * actualPassword - пароль который в данный момент подобрал хакер
     * expectedPassword - пароль от сейфа
     **/
    for (int i = 0; i < actualPasswordSize; ++i) {
        if (actualPassword[i] != expectedPassword[i]) return false;
    }

    return true;
}

char getRandomChar() {
    return symbols[rand() % symbolsCount];
}

// основная логика подбора пароля
void startCompetition(int hackerId, char* password, int passwordSize) {
    char supposedPassword[passwordSize + 1];
    supposedPassword[passwordSize] = '\0';
    int passwordPointer = 0;

    char hackerName[256];

    sprintf(hackerName, "Hacker %d", hackerId);
    cout << hackerName << " приступил к взлому" << endl;

    // для красоты заполняем звездочками
    for (int i = 0; i < passwordSize; ++i) {
        supposedPassword[i] = '*';
    }

    // С++ ОЧЕНЬ БЫСТРЫЙ язык, поэтому надо дать небольшую задержку, чтобы ОС создала потоки и положила их в пул ожидающих выполнения
    this_thread::sleep_for(chrono::milliseconds(3000));

    while (strcmp(winnerName, "") == 0) {
        supposedPassword[passwordPointer] = getRandomChar();

        if (startWith(supposedPassword, passwordPointer + 1, password)) {
            if (passwordPointer + 1 == passwordSize) {
                strcpy(winnerName, hackerName);
            }
            passwordPointer++;
        }
        // выводится на какой стадии сейчас хакер (Имя хакера: его текущий пароль)
        cout << hackerName << ": " << supposedPassword << endl;
    }
}

int main() {
    srand(time(nullptr));

    short hackerCount;

    do {
        cout << "Игроков должно быть больше чем один" << endl;
        cout << "Введите кол-во участников: ";
        cin >> hackerCount;
    } while (hackerCount <= 1);

    cout << endl;

    // пароль 100 КБ
    char password[2048 * 100];

    do {
        cout << "Пароль должен быть не пустым" << endl;
        cout << "Введите пароль от сейфа: ";
        cin >> password;
    } while (strcmp(password, "") == 0);

    thread hackers[hackerCount];

    for (int i = 0; i < hackerCount; ++i) {
        hackers[i] = thread(startCompetition, i + 1, password, strlen(password));
    }

    while (strcmp(winnerName, "") == 0) {}

    for (auto& hacker: hackers) {
        hacker.join();
    }

    cout << "\nПобедитель соревнования: " << winnerName << endl;

    return 0;
}
