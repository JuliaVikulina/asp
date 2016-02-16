/*
 * CBaseDeviceFactory.h
 *
 *  Created on: 10 февр. 2016 г.
 *      Author: alex
 */

#ifndef CBASEDEVICEFACTORY_H_
#define CBASEDEVICEFACTORY_H_

#include "CBaseDevice.h"

class CBaseDeviceFactory
{
	CBaseDeviceFactory() {}

	boost::mutex mut;

public:

	static CBaseDeviceFactory& getFactory()
	{
		static CBaseDeviceFactory* ptr = nullptr;

		boost::mutex::scoped_lock(mut);

		if (ptr == nullptr) ptr = new CBaseDeviceFactory;

		return *ptr;
	}


	IAbstractDevice* deviceFactory(const std::string& abstractName, const std::string& devName)
	{
		std::vector<IAbstractDevice*> devs;

		IAbstractDevice* dev = nullptr;

		dev = createAbstractDevice<AbstractShlagbaum>(abstractName, devName);
		if (dev) devs.push_back(dev);

//		dev = createAbstractDevice<AbstractPrinter>(abstractName, devName);
//		if (dev) devs.push_back(dev);
//		dev = createAbstractDevice<AbstractPassSensor>(abstractName, devName);
//		if (dev) devs.push_back(dev);
//		dev = createAbstractDevice<AbstractPresentSensor>(abstractName, devName);
//		if (dev) devs.push_back(dev);
//		dev = createAbstractDevice<AbstractDisplay>(abstractName, devName);
//		if (dev) devs.push_back(dev);
//		dev = createAbstractDevice<AbstractMassStorage>(abstractName, devName);
//		if (dev) devs.push_back(dev);
//		dev = createAbstractDevice<AbstractKKM>(abstractName, devName);
//		if (dev) devs.push_back(dev);

		if (devs.size() > 1)
		{
			std::cout << "deviceFactory detected device configuration error: more than 1 abstract-concrete device created" << std::endl;

			for (IAbstractDevice* d: devs)
				delete d;

			return nullptr;
		}

		if (devs.size() == 0)
		{
			std::cout << "deviceFactory detected device configuration error: no device created" << std::endl;

			return nullptr;
		}

		std::cout << "deviceFactory created device" << abstractName << std::endl;

		return dev;
	}

private:
	template<class T>
	const std::string& abstractClassName()
	{
		return T::abstractName;
	}

	template<class T>
	IAbstractDevice* createAbstractDevice(const std::string& abstractName, const std::string& devName)
	{
		IAbstractDevice* dev = nullptr;

		if ( devName.find(T::s_abstractName) != std::string::npos)
		{
			std::cout << "Create abstract device: " << abstractName << std::endl;

			dev = T::createDevice(abstractName, devName);
		}

		return dev;
	}

};


#endif /* CBASEDEVICEFACTORY_H_ */
