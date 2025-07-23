#!/usr/bin/ruby
# coding: utf-8

require 'yaml'
require 'json'

unless File::file?(ARGV[0])
    puts "文件不存在"
    exit 1
end

扩展名 = File.extname(ARGV[0])
路径 = File.dirname(ARGV[0])
文件名 = File.basename(ARGV[0], 扩展名)
if 扩展名 == '.json'
    数据 = JSON.parse(File.read(ARGV[0]))
    File.open("#{路径}/#{文件名}.yaml", "w") do |fd|
        fd.write(数据.to_yaml)
    end
elsif 扩展名 == '.yaml'
    数据 = YAML.load(File.read(ARGV[0]))
    File.open("#{路径}/#{文件名}.json", "w") do |fd|
        fd.write(数据.to_json)
    end
else
    puts "格式不支持"
    exit 1
end
