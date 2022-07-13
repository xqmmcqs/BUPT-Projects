package org.example

import org.apache.spark.rdd.RDD
import org.apache.spark.{SparkConf, SparkContext}

class ScalaWordCount {
}

object ScalaWordCount {
  def main(args: Array[String]): Unit = {
    val list = List("hello hi hi spark",
      "hello spark hello hi sparksql",
      "hello hi hi sparkstreaming",
      "hello hi sparkkqraphx")
    val sparkConf = new SparkConf().setAppName("word-count").setMaster("yarn")
    val sc = new SparkContext(sparkConf)
    val lines: RDD[String] = sc.parallelize(list)
    val words: RDD[String] = lines.flatMap((line: String) => {
      line.split(" ")
    })
    val wordAndOne: RDD[(String, Int)] = words.map((word: String) => {
      (word, 1)
    })
    val wordAndNum: RDD[(String, Int)] = wordAndOne.reduceByKey((count1: Int, count2: Int) => {
      count1 + count2
    })
    val ret = wordAndNum.sortBy(kv => kv._2, false)
    print(ret.collect().mkString(","))
    ret.saveAsTextFile("hdfs://mzh-2019211397-0001:8020/spark-test")
    sc.stop()
  }
}
