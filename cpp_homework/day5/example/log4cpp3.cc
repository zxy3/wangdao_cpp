
#include <log4cpp/Category.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/PatternLayout.hh>

#include <iostream>
using std::cout;
using std::endl;

using namespace log4cpp;

int main(void)
{
	PatternLayout *ptnLayout1 = new PatternLayout();
	ptnLayout1->setConversionPattern("%d [%p] %m%n");

	PatternLayout *ptnLayout2 = new PatternLayout();
	ptnLayout2->setConversionPattern("%d [%p] %m%n");

	PatternLayout *ptnLayout3 = new PatternLayout();
	ptnLayout3->setConversionPattern("%d [%p] %m%n");

	OstreamAppender *ostreamAppender = new OstreamAppender("ostreamAppender", &cout);
	ostreamAppender->setLayout(ptnLayout1);

	FileAppender *fileAppender = new FileAppender("fileAppender", "wangdao.log");
	fileAppender->setLayout(ptnLayout2);

	// 回卷文件
	RollingFileAppender *rollingFileAppender = new RollingFileAppender("rollingfileAppender",
																	   "rolling123.log",
																	   2 * 1024,
																	   5);
	// 输出到回卷文件中
	rollingFileAppender->setLayout(ptnLayout1);

	Category &root = Category::getRoot();
	root.setAppender(ostreamAppender);
	root.addAppender(fileAppender);
	root.addAppender(rollingFileAppender);

	root.setPriority(Priority::DEBUG); //日志记录器的级别

	int cnt = 100;
	while (cnt--)
	{
		root.alert("alert message"); //一条日志的级别
		root.crit("crit message");
		root.debug("debug message");
		root.error("error message");
		root.emerg("emerg message");
		root.info("info message");
		root.fatal("fatal message");
		root.warn("warn message");
	}

	Category::shutdown(); //回收资源

	return 0;
}
