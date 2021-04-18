package logger

import (
	rotatelogs "github.com/lestrrat-go/file-rotatelogs"
	"go.uber.org/zap"
	"go.uber.org/zap/zapcore"
	"os"
	"fmt"
	"runtime"

	//"gopkg.in/natefinch/lumberjack.v2"
	"time"
)

// error logger
var errorLogger *zap.SugaredLogger

var levelMap = map[string]zapcore.Level{
	"debug":  zapcore.DebugLevel,
	"info":   zapcore.InfoLevel,
	"warn":   zapcore.WarnLevel,
	"error":  zapcore.ErrorLevel,
	"dpanic": zapcore.DPanicLevel,
	"panic":  zapcore.PanicLevel,
	"fatal":  zapcore.FatalLevel,
}

func getLoggerLevel(lvl string) zapcore.Level {
	if level, ok := levelMap[lvl]; ok {
		return level
	}
	return zapcore.InfoLevel
}

func TimeEncoder(t time.Time, enc zapcore.PrimitiveArrayEncoder) {
	enc.AppendString(t.Format("2006-01-02 15:04:05.000"))
}

/**
初始化日志 如果初始化失败会退出程序
 */
func MustInitLog(logName, logLevel, logPath string, stdoutEnableFlag bool) {
	//pid :=  strconv.Itoa(os.Getpid())
	//fileName := logPath + logName + "-"+ pid + ".log"
	//20200511 改成按天切换日志
	fileName := logPath + logName + ".log"
	level := getLoggerLevel(logLevel)

	hook, err := rotatelogs.New(
		fileName + ".%Y%m%d",
		rotatelogs.WithLinkName(fileName),
		rotatelogs.WithMaxAge(time.Hour * 24 * 31),
		rotatelogs.WithRotationTime(time.Hour * 12),
	)

	syncWriter := zapcore.AddSync(hook)
	if err != nil {
		panic(err)
	}


	realWriter := syncWriter
	if stdoutEnableFlag {
		realWriter = zapcore.NewMultiWriteSyncer(zapcore.AddSync(os.Stdout), syncWriter)
	}

	/*
		//这个是使用lumberjack切换日志的 只能限制文件大小
		syncWriter := zapcore.AddSync(&lumberjack.Logger{
			Filename:  fileName,
			MaxSize:   10, //10m
			LocalTime: true,
			MaxAge: 7,
			Compress:  false,
		})
	*/
	encoder := zapcore.EncoderConfig{
		TimeKey:        "T",
		LevelKey:       "L",
		NameKey:        "N",
		CallerKey:      "C",
		MessageKey:     "M",
		StacktraceKey:  "S",
		LineEnding:     zapcore.DefaultLineEnding,
		EncodeLevel:    zapcore.CapitalLevelEncoder,
		EncodeTime:     TimeEncoder,
		EncodeDuration: zapcore.StringDurationEncoder,
		EncodeCaller:   zapcore.ShortCallerEncoder,
	}

	core := zapcore.NewCore(zapcore.NewConsoleEncoder(encoder), realWriter, zap.NewAtomicLevelAt(level))
	logger := zap.New(core, zap.AddCaller(), zap.AddCallerSkip(1))
	errorLogger = logger.Sugar()
}

func Debug(args ...interface{}) {
	errorLogger.Debug(args...)
}

func Debugf(template string, args ...interface{}) {
	errorLogger.Debugf(template, args...)
}

func Info(args ...interface{}) {
	errorLogger.Info(args...)
}

func Infof(template string, args ...interface{}) {
	errorLogger.Infof(template, args...)
}

func Warn(args ...interface{}) {
	errorLogger.Warn(args...)
}

func Warnf(template string, args ...interface{}) {
	errorLogger.Warnf(template, args...)
}

func Error(args ...interface{}) {
	errorLogger.Error(args...)
}

func Errorf(template string, args ...interface{}) {
	errorLogger.Errorf(template, args...)
}

func DPanic(args ...interface{}) {
	errorLogger.DPanic(args...)
}

func DPanicf(template string, args ...interface{}) {
	errorLogger.DPanicf(template, args...)
}

//发生Panic时写日志用，会同时输出堆栈信息
func ErrorOnPanic(args ...interface{}) {
	var buf [4096]byte
	//false时只获取本协程堆栈
	n := runtime.Stack(buf[:], false)

	errorLogger.Error(args...)

	errorLogger.Error(string(buf[:n]))
}

func Panic(args ...interface{}) {
	errorLogger.Panic(args...)
}

func Panicf(template string, args ...interface{}) {
	errorLogger.Panicf(template, args...)
}

func Fatal(args ...interface{}) {
	fmt.Println("发生严重异常，程序退出。详情见日志")
	errorLogger.Fatal(args...)
}

func Fatalf(template string, args ...interface{}) {
	fmt.Println("发生严重异常，程序退出。详情见日志")
	errorLogger.Fatalf(template, args...)
}
