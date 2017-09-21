package org.puder.virtualnativefs;

import android.content.Context;
import android.support.test.InstrumentationRegistry;
import android.support.test.runner.AndroidJUnit4;
import android.util.Log;

import org.junit.Test;
import org.junit.runner.RunWith;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

@RunWith(AndroidJUnit4.class)
public class VirtualNativeFSTest {

    final private static String TAG = "VirtualNativeFSTest";

    static {
        VirtualNativeFS.touch();
    }

    native private static int testFileNotExists();
    native private static int testRemoveFail();
    native private static int testCreateAndDeleteFile();
    native private static int testBasicReadWrite();
    native private static int testAppendToFile();
    native private static int testGetc();
    native private static int testPutc();
    native private static int testTell();
    native private static int testSeekInFile();
    native private static int testUpdateFile();
    native private static int testFlush();
    native private static int testTruncate();
    native private static int testRewind();
    native private static int testPrintf();
    native private static int testScanf();
    native private static int testStat();

    private void testResult(String name, int result) {
        if (result > 0) {
            String msg = "Failed: " + name + "[" + result + "]";
            Log.d(TAG, msg);
            assertTrue(msg, false);
        }
    }

    @Test
    public void useAppContext() throws Exception {
        // Context of the app under test.
        Context appContext = InstrumentationRegistry.getTargetContext();

        assertEquals("org.puder.virtualnativefs", appContext.getPackageName());
    }

    @Test
    public void runNativeTests() {
        testResult("testFileNotExists", testFileNotExists());
        testResult("testRemoveFail", testRemoveFail());
        testResult("testCreateAndDeleteFile", testCreateAndDeleteFile());
        testResult("testBasicReadWrite", testBasicReadWrite());
        testResult("testAppendToFile", testAppendToFile());
        testResult("testGetc", testGetc());
        testResult("testPutc", testPutc());
        testResult("testTell", testTell());
        testResult("testSeekInFile", testSeekInFile());
        testResult("testUpdateFile", testUpdateFile());
        testResult("testFlush", testFlush());
        testResult("testTruncate", testTruncate());
        testResult("testRewind", testRewind());
        testResult("testPrintf", testPrintf());
        testResult("testScanf", testScanf());
        testResult("testStat", testStat());
    }
}