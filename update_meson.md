Default meson version on Ubuntu24.04 is 1.3.2, but need 1.5.0 or later to run
`regenerate-meson`, so need to update meson version, 經過測試, 安裝 ubuntu
25.04 預設的 meson 版本 (1.7.0) 最好

```bash
meson --version
sudo apt update
sudo apt install python3-pip python3-setuptools python3-wheel ninja-build
sudo apt install pipx
pipx ensurepath
source ~/.bashrc
pipx install meson==1.7.0 --force
meson --version
重開機
```

```bash
ted@TedPC:/media/g2/openbmc$ meson --version
1.7.0
```

不然有可以出現以下格式化的差異

```bash
ted@TedPC:/media/g2/openbmc/meta-ted/recipes-phosphor/dbus/phosphor-dbus-interfaces/gen$ git diff '/media/g2/openbmc/meta-ted/recipes-phosphor/dbus/phosphor-dbus-interfaces/gen/com/ibm/meson.build'
diff --git a/gen/com/ibm/meson.build b/gen/com/ibm/meson.build
index 8bd25da..794060d 100644
--- a/gen/com/ibm/meson.build
+++ b/gen/com/ibm/meson.build
@@ -9,7 +9,9 @@ sdbusplus_current_path = 'com/ibm'

 generated_markdown += custom_target(
     'com/ibm/VPD__markdown'.underscorify(),
-    input: ['../../../yaml/com/ibm/VPD.errors.yaml'],
+    input: [
+        '../../../yaml/com/ibm/VPD.errors.yaml',
+    ],
     output: ['VPD.md'],
     depend_files: sdbusplusplus_depfiles,
     command: [
ted@TedPC:/media/g2/openbmc/meta-ted/recipes-phosphor/dbus/phosphor-dbus-interfaces/gen$
```
