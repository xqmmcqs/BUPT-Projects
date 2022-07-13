package org.example

import org.apache.spark.rdd.RDD
import org.apache.spark.sql.jdbc.{JdbcDialect, JdbcDialects}
import org.apache.spark.sql.{Row, SparkSession}

class ScalaWordCount {
}

object ScalaWordCount {
  def main(args: Array[String]): Unit = {
    val spark = SparkSession.builder().appName("word-count").getOrCreate()
    register()
    val df = spark.read
      .format("jdbc")
      .option("driver", "org.apache.hive.jdbc.HiveDriver")
      .option("url", "jdbc:hive2://mzh-2019211397-0001:10000/spark;auth=noSasl")
      .option("user", "root")
      .option("fetchsize", "2000")
      .option("dbtable", "spark.wordcount")
      .load()
    df.show(10)

    val lines: RDD[String] = df.rdd.map((row: Row) => {
      row.get(0).toString
    })
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
    ret.saveAsTextFile("hdfs://mzh-2019211397-0001:8020/spark/result")
    spark.stop()
  }

  def register(): Unit = {
    JdbcDialects.registerDialect(HiveSqlDialect)
  }

  case object HiveSqlDialect extends JdbcDialect {
    override def canHandle(url: String): Boolean = url.startsWith("jdbc:hive2")

    override def quoteIdentifier(colName: String): String = {
      colName.split('.').map(part => s"`$part`").mkString(".")
    }
  }
}
