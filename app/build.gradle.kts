plugins {
    alias(libs.plugins.android.library)
}

android {
    namespace = "de.lighty.unisplicemono"
    compileSdk = 35

    defaultConfig {
        minSdk = 26
        ndk {
            abiFilters += listOf("armeabi-v7a", "arm64-v8a")
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
        }
    }

    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }

    buildFeatures {
        prefab = true
    }

}

dependencies {
    implementation("com.bytedance.android:shadowhook:1.0.10")
}
