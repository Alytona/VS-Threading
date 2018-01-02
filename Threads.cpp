# include "stdafx.h"
# include "Threads.h"

// Конструктору передается размер буфера
TTransmittingBuffer::TTransmittingBuffer( int size ) {

	iSize = size;

	// Динамическое выделение буфера
	pBuffer = new double[ iSize ];

	iBeginPos = iEndPos = 0;

	// Создаем мьютекс
	rMutex = gcnew Mutex;
}
//-------------------------------------------------------------------

TTransmittingBuffer::~TTransmittingBuffer() {
	
	delete[] pBuffer;
	delete rMutex;
}
//-------------------------------------------------------------------


// Метод, который делает буфер пустым
void TTransmittingBuffer::init() {

	// Попытка захватить мьютекс перед критичным кодом
	rMutex->WaitOne();

	iBeginPos = iEndPos = 0;

	// Освобождаем мьютекс после критичного кода
	rMutex->ReleaseMutex();
}
//-------------------------------------------------------------------

// Метод возвращает процент заполненности выделенного буфера
float TTransmittingBuffer::fillingInPercent() {

	int bpos = 0, epos = 0, buffer_size = 0;

	// Попытка захватить мьютекс перед критичным кодом
	rMutex->WaitOne();

	// Зачитываем значения из разделяемых переменных, чтобы минимизировать время захвата мьютекса
	bpos = iBeginPos;
	epos = iEndPos;

	// Освобождаем мьютекс после критичного кода
	rMutex->ReleaseMutex();

	// Вычисляем заполненность буфера
	if (bpos <= epos) buffer_size = epos - bpos;
	else buffer_size = (iSize - bpos) + epos;

	return (float)(buffer_size)*100.0/(float)iSize;
}
//-------------------------------------------------------------------

// Метод возвращает текущее значение в начальной позиции, 
// передвигает указатель начальной позиции на следующую ячейку массива
double TTransmittingBuffer::peekOneValue() {

	double value = 0;

	// Попытка захватить мьютекс перед критичным кодом
	// Критичным считается весь код, где используются указатели позиций
	rMutex->WaitOne();

	// Перехватываем возможные исключения, чтобы мьютекс после кода гарантированно освободился
	try {

		value = pBuffer[iBeginPos];
		if (iBeginPos == iSize-1) iBeginPos = 0;
		else 
			iBeginPos++;

	} catch (...) {}

	// Освобождаем мьютекс после критичного кода
	rMutex->ReleaseMutex();

	return value;
}
//-------------------------------------------------------------------

// Метод пишет значение в конец буфера, 
// передвигает указатель конечной позиции на следующую ячейку
// Возвращает false, если буфер полон

bool TTransmittingBuffer::addOneValue( double value ) {

	bool res = false;

	// BeginPos - первая заполненная позиция в буфере (0-499999)
	// EndPos - первая незаполненная позиция в буфере (0-499999)
	// EndPos == BeginPos - буфер пуст
	// EndPos == 499999 && BeginPos == 0 || EndPos == BeginPos-1 - буфер полон

	// Попытка захватить мьютекс перед критичным кодом
	// Критичным считается весь код, где используются указатели позиций
	rMutex->WaitOne();

	// Перехватываем возможные исключения, чтобы мьютекс после кода гарантированно освободился
	try {

		// Добавление значения после iBeginPos
		if (iEndPos >= iBeginPos && iEndPos <= iSize-1) {
			
			pBuffer[iEndPos] = value;

			if (iEndPos == iSize-1) iEndPos = 0;
			else iEndPos++;

			res = true;
		}
		else
			// Добавление значения перед iBeginPos
			if (iEndPos < iBeginPos-1) {

				pBuffer[iEndPos] = value;
				iEndPos++;

				res = true;
			}

	// Обработка исключений
	} catch (...) {}

	// Освобождаем мьютекс после критичного кода
	rMutex->ReleaseMutex();

	return res;
}
//-------------------------------------------------------------------

// Возвращает true, если буфер полон
bool TTransmittingBuffer::isFull()
{
	bool res = true;

	// Попытка захватить мьютекс перед критичным кодом
	rMutex->WaitOne();

	// Перехватываем возможные исключения, чтобы мьютекс после кода гарантированно освободился
	try {

		res = (iEndPos == iSize-1 && iBeginPos == 0 || iEndPos == iBeginPos-1); 

	// Обработка исключений
	} catch (...) {}

	// Освобождаем мьютекс после критичного кода
	rMutex->ReleaseMutex();

	return res;
}
//-------------------------------------------------------------------

// Возвращает true, если буфер пуст
bool TTransmittingBuffer::isEmpty()
{
	bool res = true;

	// Попытка захватить мьютекс перед критичным кодом
	rMutex->WaitOne();

	// Перехватываем возможные исключения, чтобы мьютекс после кода гарантированно освободился
	try {

		res = (iBeginPos == iEndPos); 

	// Обработка исключений
	} catch (...) {}

	// Освобождаем мьютекс после критичного кода
	rMutex->ReleaseMutex();

	return res;
}
//-------------------------------------------------------------------

// Конструктору передается ссылка на буфер, созданный ранее
TTransmitterThread::TTransmitterThread( TTransmittingBuffer ^buffer )
{
	refBuffer = buffer;

	rPauseMutex = gcnew Mutex;
	rQuantityMutex = gcnew Mutex;
} 
//-------------------------------------------------------------------

TTransmitterThread::~TTransmitterThread()
{
	delete rPauseMutex;
	delete rQuantityMutex;
}

// Метод для чтения количества значений, которое может принять приемник
// Испольуется в потоке таймера формы.
int TTransmitterThread::getQuantity()
{
	int quantity = 0;

	// Попытка захватить мьютекс перед критичным кодом
	rQuantityMutex->WaitOne();

	// Критичным считается весь код, где происходит обращение к количеству значений, которые может принять приемник
	quantity = iQuantity;

	// Освобождаем мьютекс после критичного кода
	rQuantityMutex->ReleaseMutex();

	return quantity;
}
//-------------------------------------------------------------------

// Запуск потока, передается интервал отправки
void TTransmitterThread::start( int interval )
{
	iInterval = interval;

	// Открываем файл вывода
	outFile = IO::File::CreateText( ".\\out" );

	// Создаем поток передатчика и запускаем его
	threadTransmitter = gcnew System::Threading::Thread( 
		gcnew ThreadStart( this, &TTransmitterThread::threadCode ) );
	threadTransmitter->Start();
}
//-------------------------------------------------------------------

// Приостановка потока
void TTransmitterThread::pause()
{
	// Захватываем мьютекс приостановки потока передачи
	rPauseMutex->WaitOne();

	// Сбрасываем буфер файла на диск
	outFile->Flush();
}
//-------------------------------------------------------------------

// Возобновление потока после паузы
void TTransmitterThread::resume()
{
	// Освобождаем мьютекс приостановки потока передачи
	rPauseMutex->ReleaseMutex();
}
//-------------------------------------------------------------------

// Останов потока
void TTransmitterThread::stop()
{
	// Прерываем поток передачи
	threadTransmitter->Abort();
	delete threadTransmitter;

	// Сбрасываем буфер файла на диск и закрываем файл
	outFile->Flush();
	outFile->Close();
}
//-------------------------------------------------------------------

// Поток передачи
void TTransmitterThread::threadCode()
{
	// Инициализируем ГСЧ
	Random random_number;

	int quantity = 0;

	// Циклим, пока поток не прервут из основного потока
	while( 1 )
	{
		// Генерируем следующее количество значений, которое может принять приемник
		quantity = random_number.Next( 10000, 100000 );
		// Попытка захватить мьютекс перед критичным кодом
		// Критичным считается код, где происходит обращение к количеству значений, которые может принять приемник
		rQuantityMutex->WaitOne();
		iQuantity = quantity;
		// Освобождаем мьютекс после критичного кода
		rQuantityMutex->ReleaseMutex();

		// "Отправляем" требуемое количество значений
		for (int i=0; i<quantity; i++)
		{
			// Если вдруг генератор запазывает (буфер пуст), ждем когда он догонит
			while (refBuffer->isEmpty())
				// Небольшая задержка, важная вещь, 
				// без нее цикл гарантированно приведет к 100%-й загрузке процессора
				System::Threading::Thread::Sleep( 10 );

			// Сохраняем значение в файл
			outFile->WriteLine( String::Format( "{0,5:N6}", refBuffer->peekOneValue() ) );

			// Обработка паузы
			// Если попытаться захватить мьютекс, то это приведет к ожиданию пока он не освободится
			rPauseMutex->WaitOne();
			// После захвата мьютекса его необходимо освободить
			rPauseMutex->ReleaseMutex();
		}

		// Сбрасываем на диск буфер файла
		outFile->Flush();
		
		// Выдерживаем интервал отправки
		System::Threading::Thread::Sleep( iInterval );
	}
}
//-------------------------------------------------------------------

// Конструктору передается ссылка на буфер, созданный ранее
TGeneratorThread::TGeneratorThread( TTransmittingBuffer ^buffer )
{
	refBuffer = buffer;

	// Создаем мьютекс приостановки потока генератора
	rPauseMutex = gcnew Mutex;
}
//-------------------------------------------------------------------

TGeneratorThread::~TGeneratorThread()
{
	delete rPauseMutex;
}
//-------------------------------------------------------------------

// Запуск генератора, передается дискретизация сигнала
void TGeneratorThread::start( int discreet )
{
	iDiscreet = discreet;

	// Инициализация буфера перез запуском
	refBuffer->init();

	// Создаем поток генератора и запускаем его
	threadGenerator = gcnew System::Threading::Thread( 
		gcnew ThreadStart( this,  &TGeneratorThread::threadCode ) );
	threadGenerator->Start();
}
//-------------------------------------------------------------------

// Приостановка потока
void TGeneratorThread::pause()
{
	// Захватываем мьютекс приостановки потока передачи
	rPauseMutex->WaitOne();
}
//-------------------------------------------------------------------

// Возобновление потока после паузы
void TGeneratorThread::resume()
{
	// Освобождаем мьютекс приостановки потока передачи
	rPauseMutex->ReleaseMutex();
}
//-------------------------------------------------------------------

// Останов потока
void TGeneratorThread::stop()
{
	// Прерываем поток
	threadGenerator->Abort();

	// И удаляем его
	delete threadGenerator;
}
//-------------------------------------------------------------------

// Функция вычисления следующего значения 
double nextValue( double value, double &delta ) {

	// Если функция в пределах, значение получается прибавлением приращения
	if (delta > 0 && value < 1.0 || delta < 0 && value > 0.0);
	else
	{
		// При достижении функцией верхнего или нижнего предела меняем знак у приращения
		if (value > 1.0) value = 1.0;
		if (value < 0.0) value = 0.0;
		delta =- delta;
	}

	// Перед возвратом значение функции округляется до 9-го знака по правилам математики
	return System::Math::Round( value + delta, 9 );
}
//-------------------------------------------------------------------

// Поток генератора
void TGeneratorThread::threadCode()
{
	double 
		value = 0.0,					// Начальное значение функции 
		delta = 0.5/(double)iDiscreet;	// Приращение, зависит от дискретизации
	
	// Цикл, пока поток не прервут из основного потока
	while (1) {

		// Обработка паузы
		// Если попытаться захватить мьютекс, то это приведет к ожиданию пока он не освободится
		rPauseMutex->WaitOne();
		// После захвата мьютекса его необходимо освободить
		rPauseMutex->ReleaseMutex();

		// Если буфер полон, ждем когда он освободится
		if (refBuffer->isFull())
			// Небольшая задержка, важная вещь, 
			// без нее цикл гарантированно приведет к 100%-й загрузке процессора
			System::Threading::Thread::Sleep( 10 ); 

		// Если буфер не полон, то генерируем значение
		else {
			refBuffer->addOneValue( value );
			value = nextValue( value, delta );
		}
	}
}
//-------------------------------------------------------------------
