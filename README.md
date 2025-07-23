# yolo tagging tool

当时因为没有好用的 yolo 数据标记工具临时写的一个

## 使用流程

1. 先用 `yaml_json.rb` 把 yolo 的 yaml 文件转成 josn
`yaml_json.rb xxx.yaml`

2. 执行 yolo_data.exe 后选择刚刚转换后的json
3. 进行数据标记
4. 再用 `yaml_json.rb` 把 json 转回 yaml
`yaml_json.rb xxx.json`
