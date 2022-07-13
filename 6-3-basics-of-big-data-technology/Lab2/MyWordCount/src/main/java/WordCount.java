import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

import java.io.IOException;
import java.util.StringTokenizer;

public class WordCount {
    public static class TokenizerMapper extends Mapper<Object, Text, Text, IntWritable> {
        private final static IntWritable one = new IntWritable(1); // 定义常量1
        private Text word = new Text();

        public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
            StringTokenizer itr = new StringTokenizer(value.toString()); // 迭代每一个Token
            while (itr.hasMoreTokens()) {
                word.set(itr.nextToken()); // 设置word为下一个Token
                context.write(word, one); // 写入键值对{word: 1}
            }
        }
    }

    public static class IntSumReducer extends Reducer<Text, IntWritable, Text, IntWritable> {
        private IntWritable result = new IntWritable();

        public void reduce(Text key, Iterable<IntWritable> values, Context context) throws IOException, InterruptedException {
            int sum = 0;
            for (IntWritable val : values) { // 迭代键值对{key: [values]}
                sum += val.get(); // 求[values]的和
            }
            result.set(sum);
            context.write(key, result); // 写入键值对{key: sum}
        }
    }

    public static void main(String[] args) throws Exception {
        Configuration conf = new Configuration(); // 系统配置信息
        String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs(); // 运行参数
        if (otherArgs.length != 2) { // 运行参数必须恰好为2个
            System.err.println("Usage: WordCount <in> <out>");
            System.exit(2);
        }
        Job job = new Job(conf, "word count");
        job.setJarByClass(WordCount.class);
        job.setMapperClass(TokenizerMapper.class); // Mapper类
        job.setCombinerClass(IntSumReducer.class); // Combiner类
        job.setReducerClass(IntSumReducer.class); // Reducer类
        job.setOutputKeyClass(Text.class); // 输出的key类型
        job.setOutputValueClass(IntWritable.class); // 输出的value类型
        FileInputFormat.addInputPath(job, new Path(otherArgs[0])); // 从参数中构建输入文件
        FileOutputFormat.setOutputPath(job, new Path(otherArgs[1])); // 从参数中构建输出文件
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}
