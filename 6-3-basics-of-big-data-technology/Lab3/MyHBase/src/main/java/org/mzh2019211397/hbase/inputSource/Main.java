package org.mzh2019211397.hbase.inputSource;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.client.Scan;
import org.apache.hadoop.hbase.util.Bytes;
import org.apache.hadoop.hbase.mapreduce.TableMapReduceUtil;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;

public class Main {
    public final Log LOG = LogFactory.getLog(Main.class);
    public static final String NAME = "Member Test1";
    public static final String TEMP_INDEX_PATH = "hdfs://mzh-2019211397-0001:8020/tmp/2019211397-mzh";
    public static String inputTable = "2019211397-mzh";

    public static void main(String[] args) throws Exception {
        Configuration conf = HBaseConfiguration.create();
        Scan scan = new Scan();
        scan.setBatch(0);
        scan.setCaching(10000);
        scan.setMaxVersions();
        scan.setTimeRange(System.currentTimeMillis() - 3 * 24 * 3600 * 1000L, System.currentTimeMillis());
        scan.addColumn(Bytes.toBytes("cf1"), Bytes.toBytes("keyword"));
        conf.setBoolean("mapred.map.tasks.speculative.execution", false);
        conf.setBoolean("mapred.reduce.tasks.speculative.execution", false);
        Path tmpIndexPath = new Path(TEMP_INDEX_PATH);
        FileSystem fs = FileSystem.get(conf);
        if (fs.exists(tmpIndexPath)) {
            fs.delete(tmpIndexPath, true);
        }
        Job job = new Job(conf, NAME);
        job.setJarByClass(Main.class);
        TableMapReduceUtil.initTableMapperJob(inputTable, scan, MemberMapper.class, Text.class, Text.class, job);
        job.setNumReduceTasks(0);
        job.setOutputFormatClass(TextOutputFormat.class);
        FileOutputFormat.setOutputPath(job, tmpIndexPath);
        boolean success = job.waitForCompletion(true);
        System.exit(success ? 0 : 1);
    }
}
