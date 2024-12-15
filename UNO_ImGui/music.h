#ifndef _MUSIC_H_
#define _MUSIC_H_
// "-lwinmm"

#include <objbase.h>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

class Music {
    public:
    Music(){
        // 加载音效
        mciSendString("open ./music/qiu.wav alias qiu", NULL, 0, NULL);
        mciSendString("open ./music/add2.wav alias add2", NULL, 0, NULL);
        mciSendString("open ./music/again.wav alias again", NULL, 0, NULL);
        mciSendString("open ./music/beadd.wav alias beadd", NULL, 0, NULL);
        mciSendString("open ./music/pick.wav alias pick", NULL, 0, NULL);
        mciSendString("open ./music/play.wav alias play", NULL, 0, NULL);
        mciSendString("open ./music/start.wav alias start", NULL, 0, NULL);
    }
    void music_qiu() {
        mciSendString("play qiu from 0", NULL, 0, NULL);
    }
    void music_add2() {
        mciSendString("play add2 from 0", NULL, 0, NULL);
    }
    void music_beadd() {
        mciSendString("play beadd from 0", NULL, 0, NULL);
    }
    void music_again() {
        mciSendString("play again from 0", NULL, 0, NULL);
    }
    void music_pick() {
        mciSendString("play pick from 0", NULL, 0, NULL);
    }
    void music_play() {
        mciSendString("play play from 0", NULL, 0, NULL);
    }
    void music_start() {
        mciSendString("play start from 0", NULL, 0, NULL);
    }
    ~Music(){
        mciSendString("close qiu", NULL, 0, NULL);
        mciSendString("close add2", NULL, 0, NULL);
        mciSendString("close again", NULL, 0, NULL);
        mciSendString("close beadd", NULL, 0, NULL);
        mciSendString("close pick", NULL, 0, NULL);
        mciSendString("close play", NULL, 0, NULL);
        mciSendString("close start", NULL, 0, NULL);
    }
};
// "-lwinmm"

#endif