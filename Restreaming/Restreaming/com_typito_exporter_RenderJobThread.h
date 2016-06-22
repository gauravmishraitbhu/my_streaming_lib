/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_typito_exporter_RenderJobThread */

#ifndef _Included_com_typito_exporter_RenderJobThread
#define _Included_com_typito_exporter_RenderJobThread
#ifdef __cplusplus
extern "C" {
#endif
    /*
     * Class:     com_typito_exporter_RenderJobThread
     * Method:    setupLibNative
     * Signature: (Ljava/lang/String;)I
     */
    JNIEXPORT jint JNICALL Java_com_typito_exporter_RenderJobThread_setupLibNative
    (JNIEnv *, jclass, jstring);

    /*
     * Class:     com_typito_exporter_RenderJobThread
     * Method:    startJobNative
     * Signature: ([Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;[Lcom/typito/exporter/misc/ImageSequence;)I
     */
    JNIEXPORT jint JNICALL Java_com_typito_exporter_RenderJobThread_startJobNative
    (JNIEnv *, jobject, jobjectArray, jstring, jstring, jobjectArray);

    /*
     * Class:     com_typito_exporter_RenderJobThread
     * Method:    getJobStatusNative
     * Signature: (Ljava/lang/String;)Ljava/lang/String;
     */
    JNIEXPORT jstring JNICALL Java_com_typito_exporter_RenderJobThread_getJobStatusNative
    (JNIEnv *, jobject, jstring);

    JNIEXPORT jint JNICALL Java_com_typito_exporter_RenderJobThread_doRenderJob
    (JNIEnv * env, jobject jobj, jstring jobIdJStr, jobjectArray videoPathJStrings,
        jstring outputFilePathJStr, jdoubleArray videoClipStartTimes, jdoubleArray videoClipEndTimes,
        jint canvasWidth, jint canvasHeight,
        jobjectArray imageSeqJObjects);

#ifdef __cplusplus
}
#endif
#endif
