#include <jni.h>
#include <cstdlib>

extern "C" JNIEXPORT jint JNICALL
jmain(JNIEnv* env, jobject thiz, jobjectArray args)
{
    struct cmdline
    {
        struct jstr
        {
            jstring js;
            const char* cs;
        };

        JNIEnv* env;
        jsize argc;
        jstr* args;
        const char** argv;

        cmdline(JNIEnv* env, jobjectArray args)
            : env(env), argc(0), args(nullptr), argv(nullptr)
        {
            this->argc = env->GetArrayLength(args);
            this->args = (jstr*)std::calloc(this->argc, sizeof(jstr));
            this->argv = (const char**)std::calloc(this->argc + 1, sizeof(const char*));
            if (this->args && this->argv)
            {
                for (jsize i = 0; i < this->argc; ++i)
                {
                    this->args[i].js = (jstring)env->GetObjectArrayElement(args, i);
                    this->args[i].cs = env->GetStringUTFChars(this->args[i].js, nullptr);
                    this->argv[i] = this->args[i].cs;
                }
            }
        }

        ~cmdline()
        {
            if (this->args)
            {
                for (jsize i = 0; i < this->argc; ++i)
                {
                    env->ReleaseStringUTFChars(this->args[i].js, this->args[i].cs);
                }
            }
            std::free(this->args);
            std::free(this->argv);
        }
    } const cmdline(env, args);


    extern int main(int argc, char* argv[]);
    return (jint)main((int)cmdline.argc, (char**)cmdline.argv);
}
