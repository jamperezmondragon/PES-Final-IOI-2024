#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <ctime>
#include <cassert>
#include <complex>
#include <string>
#include <cstring>
#include <chrono>
#include <random>
#include <bitset>
#include <array>
#include <climits>
#include <csignal>
#include <cstdarg>
using namespace std;

/******************************** NO TOCAR, TEMPLATE DE MANEJO DE ARCHIVOS ********************************/

#ifdef _MSC_VER
#   define NORETURN __declspec(noreturn)
#elif defined __GNUC__
#   define NORETURN __attribute__ ((noreturn))
#else
#   define NORETURN
#endif

static char __testlib_format_buffer[16777216];

#define FMT_TO_RESULT(fmt, cstr, result)  std::string result;                              \
            va_list ap;                                                                    \
            va_start(ap, fmt);                                                             \
            vsnprintf(__testlib_format_buffer, sizeof(__testlib_format_buffer), cstr, ap); \
            va_end(ap);                                                                    \
            __testlib_format_buffer[sizeof(__testlib_format_buffer) - 1] = 0;              \
            result = std::string(__testlib_format_buffer);                                 \


typedef string TResult;
const TResult _ok = "AC: Respuesta Correcta";
const TResult _wa = "WA: Respuesta Erronea";
const TResult _fail = "Fallo del evaluador; Contacta al staff!";
const TResult _points = "PA: Parcialmente Correcta";

NORETURN void quit(TResult result, string message = "") {
	fprintf(stdout, "%d\n", ((result==_ok) ? 1 : 0));
	if (!message.empty()) fprintf(stderr, "%s\n", message.c_str());
	exit(0);
}

#ifdef __GNUC__
__attribute__ ((format (printf, 2, 3)))
#endif
NORETURN void quitf(TResult result, const char* fmt, ...) {
	FMT_TO_RESULT(fmt, fmt, message);
	quit(result, message);
}

FILE *grader1in,*grader1out, *grader2in, *grader2out;

/*#ifdef __GNUC__
__attribute__ ((format (printf, 3, 4)))
#endif

NORETURN inline void die(TResult result, bool sendDie, const char* fmt, ...) { // die para comunicacion
	if (sendDie) {
	  fprintf(grader2out, "-1\n");
	  fflush(grader2out);
	}
	FMT_TO_RESULT(fmt, fmt, message);
	quit(result, message);
}*/

inline FILE* openFile(const char* name, const char* mode) {
	FILE* file = fopen(name, mode);
	if (!file) quitf(_fail, "No se pudo abrir el archivo '%s' con modo '%s'.", name, mode);
	return file;
}

inline void assertInput(bool cond, string message = "No se pudo leer el Input.") {
	if (!cond) quit(_fail, message);
}

/********************************* NO TOCAR, TEMPLATE DE MANEJO DE ARCHIVOS *********************************/

int main (int argc, char **argv) {

    /********************** TEMPLATE **********************/

    if (argc < 5) {
		quit(_fail, "Insuficiente #args para el manager.");
	}

    { // mantener en broken pipes
		//signal(SIGPIPE, SIG_IGN);
		struct sigaction sa;
		sa.sa_handler = SIG_IGN;
		sigaction(SIGPIPE, &sa, NULL);
	}

    grader1out = openFile(argv[2], "a");
	grader1in = openFile(argv[1], "r");
	grader2out = openFile(argv[4], "a");
	grader2in = openFile(argv[3], "r");

    /********************** TEMPLATE **********************/

    // leer input
    bool DUMBASS = false;
    int T;
    assertInput(scanf("%d", &T) == 1);
    if (T == -1) {
        assertInput(scanf("%d", &T) == 1);
        DUMBASS = true;
    }

    int N[T], ANS[T], ANS2[T], P_SZ[T], aux;
    vector<vector<int>> L[T];
    vector<int> PIS[T];

    for (int t = 0; t < T; t++) {
        assertInput(scanf("%d", &N[t]) == 1);

        L[t] = vector<vector<int>>(N[t]);
        for (int i = 0; i < N[t]; i++) {
            for (int j = 0; j < N[t]; j++) {
                assertInput(scanf("%d", &aux) == 1);
                L[t][i].push_back(aux);
            }
        }

        assertInput(scanf("%d", &ANS[t]) == 1);
        assertInput(scanf("%d", &ANS2[t]) == 1);
    }
    fclose(stdin);

    // mandar input
    fprintf(grader1out, "%d\n", T);
    fflush(grader1out);

    for (int t = 0; t < T; t++) {
        fprintf(grader1out, "%d\n", N[t]);
        fflush(grader1out);

        for (int i = 0; i < N[t]; i++) {
            for (int j = 0; j < N[t]; j++)
                fprintf(grader1out, "%d ", L[t][i][j]);
            fprintf(grader1out, "\n");
            fflush(grader1out);
        }
    }

    // leer el output procesado

    for (int t = 0; t < T; t++) {
        assertInput(fscanf(grader1in, "%d", &P_SZ[t]) == 1);
        for (int i = 0; i < P_SZ[t]; i++) {
            assertInput(fscanf(grader1in, "%d", &aux) == 1);
            PIS[t].push_back(aux);
        }
    }

    // saca respuesta preliminar

    float final_ans = (float)1, xx;
    for (int t = 0; t < T; t++) {
        xx = 2*(ANS[t] + 2) - P_SZ[t];
        xx = xx / ((float)2*(ANS[t] + 2) - ANS2[t]);
        final_ans = min(final_ans, xx);
    }

    final_ans = min(max(final_ans, (float)0), (float)1);

    fclose(grader1out);
	fclose(grader1in);

    // mandar al segundo stub

    fprintf(grader2out, "%d\n", T);
    fflush(grader2out);

    for (int t = 0; t < T; t++) {
        fprintf(grader2out, "%d\n", P_SZ[t]);
        fflush(grader2out);
        for (int i = 0; i < P_SZ[t]; i++)
            fprintf(grader2out, "%d\n", PIS[t][i]);
        fprintf(grader2out, "\n");
        fflush(grader2out);
    }

    for (int t = 0; t < T; t++) {
        fprintf(grader2out, "%d\n", N[t]);
        fflush(grader2out);

        for (int i = 0; i < N[t]; i++) {
            for (int j = 0; j < N[t]; j++)
                fprintf(grader2out, "%d ", L[t][i][j]);
            fprintf(grader2out, "\n");
            fflush(grader2out);
        }
    }

    for (int t = 0; t < T; t++) {
        assertInput(fscanf(grader2in, "%d", &aux) == 1);
        if (aux == -1) final_ans = 0;
        if (!DUMBASS && aux != ANS[t]) final_ans = 0;
    }

	fclose(grader2out);
	fclose(grader2in);

    fprintf(stdout, "%f\n", final_ans);
    return 0;
}
