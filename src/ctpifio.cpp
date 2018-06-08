#include "ctpifio.h"



int getLogfile(ofstream &out, const char* filepath) {
  try {
    out.open(filepath, ios_base::app | ios_base::out);
#ifdef _WINDOWS
    out.imbue(locale("chs"));
#else
    out.imbue(locale("zh_CN.UTF-8"));
#endif
    return (0);
  } catch (exception) {
    return (-1);
  }
}


int getLogfile(ofstream &out, std::string& filepath) {
  try {
    out.open(filepath, ios_base::app | ios_base::out);
#ifdef _WINDOWS
    out.imbue(locale("chs"));
#else
    out.imbue(locale("zh_CN.UTF-8"));
#endif
    return (0);
  } catch (exception) {
    return (-1);
  }
}


/* 信息分发 */
ostream &operator<<(ostream &out, CThostFtdcDisseminationField &data) {
  out << "CThostFtdcDisseminationField" << endl
      << "序列系列号, "
      << "SequenceSeries, " << data.SequenceSeries << endl
      << "序列号, "
      << "SequenceNo, " << data.SequenceNo << endl;
  return out;
}

/* 用户登录请求 */
ostream &operator<<(ostream &out, CThostFtdcReqUserLoginField &data) {
  out << "CThostFtdcReqUserLoginField" << endl
      << "交易日, "
      << "TradingDay, " << data.TradingDay << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "密码, "
      << "Password, " << data.Password << endl
      << "用户端产品信息, "
      << "UserProductInfo, " << data.UserProductInfo << endl
      << "接口端产品信息, "
      << "InterfaceProductInfo, " << data.InterfaceProductInfo << endl
      << "协议信息, "
      << "ProtocolInfo, " << data.ProtocolInfo << endl
      << "Mac地址, "
      << "MacAddress, " << data.MacAddress << endl
      << "动态密码, "
      << "OneTimePassword, " << data.OneTimePassword << endl
      << "终端IP地址, "
      << "ClientIPAddress, " << data.ClientIPAddress << endl;
  return out;
}

/* 用户登录应答 */
ostream &operator<<(ostream &out, CThostFtdcRspUserLoginField &data) {
  out << "CThostFtdcRspUserLoginField" << endl
      << "交易日, "
      << "TradingDay, " << data.TradingDay << endl
      << "登录成功时间, "
      << "LoginTime, " << data.LoginTime << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "交易系统名称, "
      << "SystemName, " << data.SystemName << endl
      << "前置编号, "
      << "FrontID, " << data.FrontID << endl
      << "会话编号, "
      << "SessionID, " << data.SessionID << endl
      << "最大报单引用, "
      << "MaxOrderRef, " << data.MaxOrderRef << endl
      << "上期所时间, "
      << "SHFETime, " << data.SHFETime << endl
      << "大商所时间, "
      << "DCETime, " << data.DCETime << endl
      << "郑商所时间, "
      << "CZCETime, " << data.CZCETime << endl
      << "中金所时间, "
      << "FFEXTime, " << data.FFEXTime << endl;
  return out;
}

/* 用户登出请求 */
ostream &operator<<(ostream &out, CThostFtdcUserLogoutField &data) {
  out << "CThostFtdcUserLogoutField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl;
  return out;
}

/* 强制交易员退出 */
ostream &operator<<(ostream &out, CThostFtdcForceUserLogoutField &data) {
  out << "CThostFtdcForceUserLogoutField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl;
  return out;
}

/* 银期转帐报文头 */
ostream &operator<<(ostream &out, CThostFtdcTransferHeaderField &data) {
  out << "CThostFtdcTransferHeaderField" << endl
      << "版本号，常量，1.0, "
      << "Version, " << data.Version << endl
      << "交易代码，必填, "
      << "TradeCode, " << data.TradeCode << endl
      << "交易日期，必填，格式：yyyymmdd, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易时间，必填，格式：hhmmss, "
      << "TradeTime, " << data.TradeTime << endl
      << "发起方流水号，N/A, "
      << "TradeSerial, " << data.TradeSerial << endl
      << "期货公司代码，必填, "
      << "FutureID, " << data.FutureID << endl
      << "银行代码，根据查询银行得到，必填, "
      << "BankID, " << data.BankID << endl
      << "银行分中心代码，根据查询银行得到，必填, "
      << "BankBrchID, " << data.BankBrchID << endl
      << "操作员，N/A, "
      << "OperNo, " << data.OperNo << endl
      << "交易设备类型，N/A, "
      << "DeviceID, " << data.DeviceID << endl
      << "记录数，N/A, "
      << "RecordNum, " << data.RecordNum << endl
      << "会话编号，N/A, "
      << "SessionID, " << data.SessionID << endl
      << "请求编号，N/A, "
      << "RequestID, " << data.RequestID << endl;
  return out;
}

/* 银行资金转期货请求，TradeCode=202001 */
ostream &operator<<(ostream &out,
                    CThostFtdcTransferBankToFutureReqField &data) {
  out << "CThostFtdcTransferBankToFutureReqField" << endl
      << "期货资金账户, "
      << "FutureAccount, " << data.FutureAccount << endl
      << "密码标志, "
      << "FuturePwdFlag, " << data.FuturePwdFlag << endl
      << "密码, "
      << "FutureAccPwd, " << data.FutureAccPwd << endl
      << "转账金额, "
      << "TradeAmt, " << data.TradeAmt << endl
      << "客户手续费, "
      << "CustFee, " << data.CustFee << endl
      << "币种：RMB-人民币 USD-美圆 HKD-港元, "
      << "CurrencyCode, " << data.CurrencyCode << endl;
  return out;
}

/* 银行资金转期货请求响应 */
ostream &operator<<(ostream &out,
                    CThostFtdcTransferBankToFutureRspField &data) {
  out << "CThostFtdcTransferBankToFutureRspField" << endl
      << "响应代码, "
      << "RetCode, " << data.RetCode << endl
      << "响应信息, "
      << "RetInfo, " << data.RetInfo << endl
      << "资金账户, "
      << "FutureAccount, " << data.FutureAccount << endl
      << "转帐金额, "
      << "TradeAmt, " << data.TradeAmt << endl
      << "应收客户手续费, "
      << "CustFee, " << data.CustFee << endl
      << "币种, "
      << "CurrencyCode, " << data.CurrencyCode << endl;
  return out;
}

/* 期货资金转银行请求，TradeCode=202002 */
ostream &operator<<(ostream &out,
                    CThostFtdcTransferFutureToBankReqField &data) {
  out << "CThostFtdcTransferFutureToBankReqField" << endl
      << "期货资金账户, "
      << "FutureAccount, " << data.FutureAccount << endl
      << "密码标志, "
      << "FuturePwdFlag, " << data.FuturePwdFlag << endl
      << "密码, "
      << "FutureAccPwd, " << data.FutureAccPwd << endl
      << "转账金额, "
      << "TradeAmt, " << data.TradeAmt << endl
      << "客户手续费, "
      << "CustFee, " << data.CustFee << endl
      << "币种：RMB-人民币 USD-美圆 HKD-港元, "
      << "CurrencyCode, " << data.CurrencyCode << endl;
  return out;
}

/* 期货资金转银行请求响应 */
ostream &operator<<(ostream &out,
                    CThostFtdcTransferFutureToBankRspField &data) {
  out << "CThostFtdcTransferFutureToBankRspField" << endl
      << "响应代码, "
      << "RetCode, " << data.RetCode << endl
      << "响应信息, "
      << "RetInfo, " << data.RetInfo << endl
      << "资金账户, "
      << "FutureAccount, " << data.FutureAccount << endl
      << "转帐金额, "
      << "TradeAmt, " << data.TradeAmt << endl
      << "应收客户手续费, "
      << "CustFee, " << data.CustFee << endl
      << "币种, "
      << "CurrencyCode, " << data.CurrencyCode << endl;
  return out;
}

/* 查询银行资金请求，TradeCode=204002 */
ostream &operator<<(ostream &out, CThostFtdcTransferQryBankReqField &data) {
  out << "CThostFtdcTransferQryBankReqField" << endl
      << "期货资金账户, "
      << "FutureAccount, " << data.FutureAccount << endl
      << "密码标志, "
      << "FuturePwdFlag, " << data.FuturePwdFlag << endl
      << "密码, "
      << "FutureAccPwd, " << data.FutureAccPwd << endl
      << "币种：RMB-人民币 USD-美圆 HKD-港元, "
      << "CurrencyCode, " << data.CurrencyCode << endl;
  return out;
}

/* 查询银行资金请求响应 */
ostream &operator<<(ostream &out, CThostFtdcTransferQryBankRspField &data) {
  out << "CThostFtdcTransferQryBankRspField" << endl
      << "响应代码, "
      << "RetCode, " << data.RetCode << endl
      << "响应信息, "
      << "RetInfo, " << data.RetInfo << endl
      << "资金账户, "
      << "FutureAccount, " << data.FutureAccount << endl
      << "银行余额, "
      << "TradeAmt, " << data.TradeAmt << endl
      << "银行可用余额, "
      << "UseAmt, " << data.UseAmt << endl
      << "银行可取余额, "
      << "FetchAmt, " << data.FetchAmt << endl
      << "币种, "
      << "CurrencyCode, " << data.CurrencyCode << endl;
  return out;
}

/* 查询银行交易明细请求，TradeCode=204999 */
ostream &operator<<(ostream &out, CThostFtdcTransferQryDetailReqField &data) {
  out << "CThostFtdcTransferQryDetailReqField" << endl
      << "期货资金账户, "
      << "FutureAccount, " << data.FutureAccount << endl;
  return out;
}

/* 查询银行交易明细请求响应 */
ostream &operator<<(ostream &out, CThostFtdcTransferQryDetailRspField &data) {
  out << "CThostFtdcTransferQryDetailRspField" << endl
      << "交易日期, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "交易代码, "
      << "TradeCode, " << data.TradeCode << endl
      << "期货流水号, "
      << "FutureSerial, " << data.FutureSerial << endl
      << "期货公司代码, "
      << "FutureID, " << data.FutureID << endl
      << "资金帐号, "
      << "FutureAccount, " << data.FutureAccount << endl
      << "银行流水号, "
      << "BankSerial, " << data.BankSerial << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分中心代码, "
      << "BankBrchID, " << data.BankBrchID << endl
      << "银行账号, "
      << "BankAccount, " << data.BankAccount << endl
      << "证件号码, "
      << "CertCode, " << data.CertCode << endl
      << "货币代码, "
      << "CurrencyCode, " << data.CurrencyCode << endl
      << "发生金额, "
      << "TxAmount, " << data.TxAmount << endl
      << "有效标志, "
      << "Flag, " << data.Flag << endl;
  return out;
}

/* 响应信息 */
ostream &operator<<(ostream &out, CThostFtdcRspInfoField &data) {
  out << "CThostFtdcRspInfoField" << endl
      << "错误代码, "
      << "ErrorID, " << data.ErrorID << endl
      << "错误信息, "
      << "ErrorMsg, " << data.ErrorMsg << endl;
  return out;
}

/* 交易所 */
ostream &operator<<(ostream &out, CThostFtdcExchangeField &data) {
  out << "CThostFtdcExchangeField" << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "交易所名称, "
      << "ExchangeName, " << data.ExchangeName << endl
      << "交易所属性, "
      << "ExchangeProperty, " << data.ExchangeProperty << endl;
  return out;
}

/* 产品 */
ostream &operator<<(ostream &out, CThostFtdcProductField &data) {
  out << "CThostFtdcProductField" << endl
      << "产品代码, "
      << "ProductID, " << data.ProductID << endl
      << "产品名称, "
      << "ProductName, " << data.ProductName << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "产品类型, "
      << "ProductClass, " << data.ProductClass << endl
      << "合约数量乘数, "
      << "VolumeMultiple, " << data.VolumeMultiple << endl
      << "最小变动价位, "
      << "PriceTick, " << data.PriceTick << endl
      << "市价单最大下单量, "
      << "MaxMarketOrderVolume, " << data.MaxMarketOrderVolume << endl
      << "市价单最小下单量, "
      << "MinMarketOrderVolume, " << data.MinMarketOrderVolume << endl
      << "限价单最大下单量, "
      << "MaxLimitOrderVolume, " << data.MaxLimitOrderVolume << endl
      << "限价单最小下单量, "
      << "MinLimitOrderVolume, " << data.MinLimitOrderVolume << endl
      << "持仓类型, "
      << "PositionType, " << data.PositionType << endl
      << "持仓日期类型, "
      << "PositionDateType, " << data.PositionDateType << endl;
  return out;
}

/* 合约 */
ostream &operator<<(ostream &out, CThostFtdcInstrumentField &data) {
  out << "CThostFtdcInstrumentField" << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "合约名称, "
      << "InstrumentName, " << data.InstrumentName << endl
      << "合约在交易所的代码, "
      << "ExchangeInstID, " << data.ExchangeInstID << endl
      << "产品代码, "
      << "ProductID, " << data.ProductID << endl
      << "产品类型, "
      << "ProductClass, " << data.ProductClass << endl
      << "交割年份, "
      << "DeliveryYear, " << data.DeliveryYear << endl
      << "交割月, "
      << "DeliveryMonth, " << data.DeliveryMonth << endl
      << "市价单最大下单量, "
      << "MaxMarketOrderVolume, " << data.MaxMarketOrderVolume << endl
      << "市价单最小下单量, "
      << "MinMarketOrderVolume, " << data.MinMarketOrderVolume << endl
      << "限价单最大下单量, "
      << "MaxLimitOrderVolume, " << data.MaxLimitOrderVolume << endl
      << "限价单最小下单量, "
      << "MinLimitOrderVolume, " << data.MinLimitOrderVolume << endl
      << "合约数量乘数, "
      << "VolumeMultiple, " << data.VolumeMultiple << endl
      << "最小变动价位, "
      << "PriceTick, " << data.PriceTick << endl
      << "创建日, "
      << "CreateDate, " << data.CreateDate << endl
      << "上市日, "
      << "OpenDate, " << data.OpenDate << endl
      << "到期日, "
      << "ExpireDate, " << data.ExpireDate << endl
      << "开始交割日, "
      << "StartDelivDate, " << data.StartDelivDate << endl
      << "结束交割日, "
      << "EndDelivDate, " << data.EndDelivDate << endl
      << "合约生命周期状态, "
      << "InstLifePhase, " << data.InstLifePhase << endl
      << "当前是否交易, "
      << "IsTrading, " << data.IsTrading << endl
      << "持仓类型, "
      << "PositionType, " << data.PositionType << endl
      << "持仓日期类型, "
      << "PositionDateType, " << data.PositionDateType << endl
      << "多头保证金率, "
      << "LongMarginRatio, " << data.LongMarginRatio << endl
      << "空头保证金率, "
      << "ShortMarginRatio, " << data.ShortMarginRatio << endl;
  return out;
}

/* 经纪公司 */
ostream &operator<<(ostream &out, CThostFtdcBrokerField &data) {
  out << "CThostFtdcBrokerField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "经纪公司简称, "
      << "BrokerAbbr, " << data.BrokerAbbr << endl
      << "经纪公司名称, "
      << "BrokerName, " << data.BrokerName << endl
      << "是否活跃, "
      << "IsActive, " << data.IsActive << endl;
  return out;
}

/* 交易所交易员 */
ostream &operator<<(ostream &out, CThostFtdcTraderField &data) {
  out << "CThostFtdcTraderField" << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "交易所交易员代码, "
      << "TraderID, " << data.TraderID << endl
      << "会员代码, "
      << "ParticipantID, " << data.ParticipantID << endl
      << "密码, "
      << "Password, " << data.Password << endl
      << "安装数量, "
      << "InstallCount, " << data.InstallCount << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl;
  return out;
}

/* 投资者 */
ostream &operator<<(ostream &out, CThostFtdcInvestorField &data) {
  out << "CThostFtdcInvestorField" << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者分组代码, "
      << "InvestorGroupID, " << data.InvestorGroupID << endl
      << "投资者名称, "
      << "InvestorName, " << data.InvestorName << endl
      << "证件类型, "
      << "IdentifiedCardType, " << data.IdentifiedCardType << endl
      << "证件号码, "
      << "IdentifiedCardNo, " << data.IdentifiedCardNo << endl
      << "是否活跃, "
      << "IsActive, " << data.IsActive << endl
      << "联系电话, "
      << "Telephone, " << data.Telephone << endl
      << "通讯地址, "
      << "Address, " << data.Address << endl
      << "开户日期, "
      << "OpenDate, " << data.OpenDate << endl
      << "手机, "
      << "Mobile, " << data.Mobile << endl;
  return out;
}

/* 交易编码 */
ostream &operator<<(ostream &out, CThostFtdcTradingCodeField &data) {
  out << "CThostFtdcTradingCodeField" << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "客户代码, "
      << "ClientID, " << data.ClientID << endl
      << "是否活跃, "
      << "IsActive, " << data.IsActive << endl
      << "交易编码类型, "
      << "ClientIDType, " << data.ClientIDType << endl;
  return out;
}

/* 会员编码和经纪公司编码对照表 */
ostream &operator<<(ostream &out, CThostFtdcPartBrokerField &data) {
  out << "CThostFtdcPartBrokerField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "会员代码, "
      << "ParticipantID, " << data.ParticipantID << endl
      << "是否活跃, "
      << "IsActive, " << data.IsActive << endl;
  return out;
}

/* 管理用户 */
ostream &operator<<(ostream &out, CThostFtdcSuperUserField &data) {
  out << "CThostFtdcSuperUserField" << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "用户名称, "
      << "UserName, " << data.UserName << endl
      << "密码, "
      << "Password, " << data.Password << endl
      << "是否活跃, "
      << "IsActive, " << data.IsActive << endl;
  return out;
}

/* 管理用户功能权限 */
ostream &operator<<(ostream &out, CThostFtdcSuperUserFunctionField &data) {
  out << "CThostFtdcSuperUserFunctionField" << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "功能代码, "
      << "FunctionCode, " << data.FunctionCode << endl;
  return out;
}

/* 投资者组 */
ostream &operator<<(ostream &out, CThostFtdcInvestorGroupField &data) {
  out << "CThostFtdcInvestorGroupField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者分组代码, "
      << "InvestorGroupID, " << data.InvestorGroupID << endl
      << "投资者分组名称, "
      << "InvestorGroupName, " << data.InvestorGroupName << endl;
  return out;
}

/* 资金账户 */
ostream &operator<<(ostream &out, CThostFtdcTradingAccountField &data) {
  out << "CThostFtdcTradingAccountField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者帐号, "
      << "AccountID, " << data.AccountID << endl
      << "上次质押金额, "
      << "PreMortgage, " << data.PreMortgage << endl
      << "上次信用额度, "
      << "PreCredit, " << data.PreCredit << endl
      << "上次存款额, "
      << "PreDeposit, " << data.PreDeposit << endl
      << "上次结算准备金, "
      << "PreBalance, " << data.PreBalance << endl
      << "上次占用的保证金, "
      << "PreMargin, " << data.PreMargin << endl
      << "利息基数, "
      << "InterestBase, " << data.InterestBase << endl
      << "利息收入, "
      << "Interest, " << data.Interest << endl
      << "入金金额, "
      << "Deposit, " << data.Deposit << endl
      << "出金金额, "
      << "Withdraw, " << data.Withdraw << endl
      << "冻结的保证金, "
      << "FrozenMargin, " << data.FrozenMargin << endl
      << "冻结的资金, "
      << "FrozenCash, " << data.FrozenCash << endl
      << "冻结的手续费, "
      << "FrozenCommission, " << data.FrozenCommission << endl
      << "当前保证金总额, "
      << "CurrMargin, " << data.CurrMargin << endl
      << "资金差额, "
      << "CashIn, " << data.CashIn << endl
      << "手续费, "
      << "Commission, " << data.Commission << endl
      << "平仓盈亏, "
      << "CloseProfit, " << data.CloseProfit << endl
      << "持仓盈亏, "
      << "PositionProfit, " << data.PositionProfit << endl
      << "期货结算准备金, "
      << "Balance, " << data.Balance << endl
      << "可用资金, "
      << "Available, " << data.Available << endl
      << "可取资金, "
      << "WithdrawQuota, " << data.WithdrawQuota << endl
      << "基本准备金, "
      << "Reserve, " << data.Reserve << endl
      << "交易日, "
      << "TradingDay, " << data.TradingDay << endl
      << "结算编号, "
      << "SettlementID, " << data.SettlementID << endl
      << "信用额度, "
      << "Credit, " << data.Credit << endl
      << "质押金额, "
      << "Mortgage, " << data.Mortgage << endl
      << "交易所保证金, "
      << "ExchangeMargin, " << data.ExchangeMargin << endl
      << "投资者交割保证金, "
      << "DeliveryMargin, " << data.DeliveryMargin << endl
      << "交易所交割保证金, "
      << "ExchangeDeliveryMargin, " << data.ExchangeDeliveryMargin << endl;
  return out;
}

/* 投资者持仓 */
ostream &operator<<(ostream &out, CThostFtdcInvestorPositionField &data) {
  out << "CThostFtdcInvestorPositionField" << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "持仓多空方向, "
      << "PosiDirection, " << data.PosiDirection << endl
      << "投机套保标志, "
      << "HedgeFlag, " << data.HedgeFlag << endl
      << "持仓日期, "
      << "PositionDate, " << data.PositionDate << endl
      << "上日持仓, "
      << "YdPosition, " << data.YdPosition << endl
      << "今日持仓, "
      << "Position, " << data.Position << endl
      << "多头冻结, "
      << "LongFrozen, " << data.LongFrozen << endl
      << "空头冻结, "
      << "ShortFrozen, " << data.ShortFrozen << endl
      << "开仓冻结金额, "
      << "LongFrozenAmount, " << data.LongFrozenAmount << endl
      << "开仓冻结金额, "
      << "ShortFrozenAmount, " << data.ShortFrozenAmount << endl
      << "开仓量, "
      << "OpenVolume, " << data.OpenVolume << endl
      << "平仓量, "
      << "CloseVolume, " << data.CloseVolume << endl
      << "开仓金额, "
      << "OpenAmount, " << data.OpenAmount << endl
      << "平仓金额, "
      << "CloseAmount, " << data.CloseAmount << endl
      << "持仓成本, "
      << "PositionCost, " << data.PositionCost << endl
      << "上次占用的保证金, "
      << "PreMargin, " << data.PreMargin << endl
      << "占用的保证金, "
      << "UseMargin, " << data.UseMargin << endl
      << "冻结的保证金, "
      << "FrozenMargin, " << data.FrozenMargin << endl
      << "冻结的资金, "
      << "FrozenCash, " << data.FrozenCash << endl
      << "冻结的手续费, "
      << "FrozenCommission, " << data.FrozenCommission << endl
      << "资金差额, "
      << "CashIn, " << data.CashIn << endl
      << "手续费, "
      << "Commission, " << data.Commission << endl
      << "平仓盈亏, "
      << "CloseProfit, " << data.CloseProfit << endl
      << "持仓盈亏, "
      << "PositionProfit, " << data.PositionProfit << endl
      << "上次结算价, "
      << "PreSettlementPrice, " << data.PreSettlementPrice << endl
      << "本次结算价, "
      << "SettlementPrice, " << data.SettlementPrice << endl
      << "交易日, "
      << "TradingDay, " << data.TradingDay << endl
      << "结算编号, "
      << "SettlementID, " << data.SettlementID << endl
      << "开仓成本, "
      << "OpenCost, " << data.OpenCost << endl
      << "交易所保证金, "
      << "ExchangeMargin, " << data.ExchangeMargin << endl
      << "组合成交形成的持仓, "
      << "CombPosition, " << data.CombPosition << endl
      << "组合多头冻结, "
      << "CombLongFrozen, " << data.CombLongFrozen << endl
      << "组合空头冻结, "
      << "CombShortFrozen, " << data.CombShortFrozen << endl
      << "逐日盯市平仓盈亏, "
      << "CloseProfitByDate, " << data.CloseProfitByDate << endl
      << "逐笔对冲平仓盈亏, "
      << "CloseProfitByTrade, " << data.CloseProfitByTrade << endl
      << "今日持仓, "
      << "TodayPosition, " << data.TodayPosition << endl
      << "保证金率, "
      << "MarginRateByMoney, " << data.MarginRateByMoney << endl
      << "保证金率(按手数), "
      << "MarginRateByVolume, " << data.MarginRateByVolume << endl;
  return out;
}

/* 合约保证金率 */
ostream &operator<<(ostream &out, CThostFtdcInstrumentMarginRateField &data) {
  out << "CThostFtdcInstrumentMarginRateField" << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "投资者范围, "
      << "InvestorRange, " << data.InvestorRange << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "投机套保标志, "
      << "HedgeFlag, " << data.HedgeFlag << endl
      << "多头保证金率, "
      << "LongMarginRatioByMoney, " << data.LongMarginRatioByMoney << endl
      << "多头保证金费, "
      << "LongMarginRatioByVolume, " << data.LongMarginRatioByVolume << endl
      << "空头保证金率, "
      << "ShortMarginRatioByMoney, " << data.ShortMarginRatioByMoney << endl
      << "空头保证金费, "
      << "ShortMarginRatioByVolume, " << data.ShortMarginRatioByVolume << endl
      << "是否相对交易所收取, "
      << "IsRelative, " << data.IsRelative << endl;
  return out;
}

/* 合约手续费率 */
ostream &operator<<(ostream &out,
                    CThostFtdcInstrumentCommissionRateField &data) {
  out << "CThostFtdcInstrumentCommissionRateField" << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "投资者范围, "
      << "InvestorRange, " << data.InvestorRange << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "开仓手续费率, "
      << "OpenRatioByMoney, " << data.OpenRatioByMoney << endl
      << "开仓手续费, "
      << "OpenRatioByVolume, " << data.OpenRatioByVolume << endl
      << "平仓手续费率, "
      << "CloseRatioByMoney, " << data.CloseRatioByMoney << endl
      << "平仓手续费, "
      << "CloseRatioByVolume, " << data.CloseRatioByVolume << endl
      << "平今手续费率, "
      << "CloseTodayRatioByMoney, " << data.CloseTodayRatioByMoney << endl
      << "平今手续费, "
      << "CloseTodayRatioByVolume, " << data.CloseTodayRatioByVolume << endl;
  return out;
}

/* 深度行情 */
ostream &operator<<(ostream &out, CThostFtdcDepthMarketDataField &data) {
  out << "CThostFtdcDepthMarketDataField" << endl
      << "交易日, "
      << "TradingDay, " << data.TradingDay << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "合约在交易所的代码, "
      << "ExchangeInstID, " << data.ExchangeInstID << endl
      << "最新价, "
      << "LastPrice, " << data.LastPrice << endl
      << "上次结算价, "
      << "PreSettlementPrice, " << data.PreSettlementPrice << endl
      << "昨收盘, "
      << "PreClosePrice, " << data.PreClosePrice << endl
      << "昨持仓量, "
      << "PreOpenInterest, " << data.PreOpenInterest << endl
      << "今开盘, "
      << "OpenPrice, " << data.OpenPrice << endl
      << "最高价, "
      << "HighestPrice, " << data.HighestPrice << endl
      << "最低价, "
      << "LowestPrice, " << data.LowestPrice << endl
      << "数量, "
      << "Volume, " << data.Volume << endl
      << "成交金额, "
      << "Turnover, " << data.Turnover << endl
      << "持仓量, "
      << "OpenInterest, " << data.OpenInterest << endl
      << "今收盘, "
      << "ClosePrice, " << data.ClosePrice << endl
      << "本次结算价, "
      << "SettlementPrice, " << data.SettlementPrice << endl
      << "涨停板价, "
      << "UpperLimitPrice, " << data.UpperLimitPrice << endl
      << "跌停板价, "
      << "LowerLimitPrice, " << data.LowerLimitPrice << endl
      << "昨虚实度, "
      << "PreDelta, " << data.PreDelta << endl
      << "今虚实度, "
      << "CurrDelta, " << data.CurrDelta << endl
      << "最后修改时间, "
      << "UpdateTime, " << data.UpdateTime << endl
      << "最后修改毫秒, "
      << "UpdateMillisec, " << data.UpdateMillisec << endl
      << "申买价一, "
      << "BidPrice1, " << data.BidPrice1 << endl
      << "申买量一, "
      << "BidVolume1, " << data.BidVolume1 << endl
      << "申卖价一, "
      << "AskPrice1, " << data.AskPrice1 << endl
      << "申卖量一, "
      << "AskVolume1, " << data.AskVolume1 << endl
      << "申买价二, "
      << "BidPrice2, " << data.BidPrice2 << endl
      << "申买量二, "
      << "BidVolume2, " << data.BidVolume2 << endl
      << "申卖价二, "
      << "AskPrice2, " << data.AskPrice2 << endl
      << "申卖量二, "
      << "AskVolume2, " << data.AskVolume2 << endl
      << "申买价三, "
      << "BidPrice3, " << data.BidPrice3 << endl
      << "申买量三, "
      << "BidVolume3, " << data.BidVolume3 << endl
      << "申卖价三, "
      << "AskPrice3, " << data.AskPrice3 << endl
      << "申卖量三, "
      << "AskVolume3, " << data.AskVolume3 << endl
      << "申买价四, "
      << "BidPrice4, " << data.BidPrice4 << endl
      << "申买量四, "
      << "BidVolume4, " << data.BidVolume4 << endl
      << "申卖价四, "
      << "AskPrice4, " << data.AskPrice4 << endl
      << "申卖量四, "
      << "AskVolume4, " << data.AskVolume4 << endl
      << "申买价五, "
      << "BidPrice5, " << data.BidPrice5 << endl
      << "申买量五, "
      << "BidVolume5, " << data.BidVolume5 << endl
      << "申卖价五, "
      << "AskPrice5, " << data.AskPrice5 << endl
      << "申卖量五, "
      << "AskVolume5, " << data.AskVolume5 << endl
      << "当日均价, "
      << "AveragePrice, " << data.AveragePrice << endl;
  return out;
}

/* 投资者合约交易权限 */
ostream &operator<<(ostream &out, CThostFtdcInstrumentTradingRightField &data) {
  out << "CThostFtdcInstrumentTradingRightField" << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "投资者范围, "
      << "InvestorRange, " << data.InvestorRange << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "交易权限, "
      << "TradingRight, " << data.TradingRight << endl;
  return out;
}

/* 经纪公司用户 */
ostream &operator<<(ostream &out, CThostFtdcBrokerUserField &data) {
  out << "CThostFtdcBrokerUserField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "用户名称, "
      << "UserName, " << data.UserName << endl
      << "用户类型, "
      << "UserType, " << data.UserType << endl
      << "是否活跃, "
      << "IsActive, " << data.IsActive << endl
      << "是否使用令牌, "
      << "IsUsingOTP, " << data.IsUsingOTP << endl;
  return out;
}

/* 经纪公司用户口令 */
ostream &operator<<(ostream &out, CThostFtdcBrokerUserPasswordField &data) {
  out << "CThostFtdcBrokerUserPasswordField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "密码, "
      << "Password, " << data.Password << endl;
  return out;
}

/* 经纪公司用户功能权限 */
ostream &operator<<(ostream &out, CThostFtdcBrokerUserFunctionField &data) {
  out << "CThostFtdcBrokerUserFunctionField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "经纪公司功能代码, "
      << "BrokerFunctionCode, " << data.BrokerFunctionCode << endl;
  return out;
}

/* 交易所交易员报盘机 */
ostream &operator<<(ostream &out, CThostFtdcTraderOfferField &data) {
  out << "CThostFtdcTraderOfferField" << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "交易所交易员代码, "
      << "TraderID, " << data.TraderID << endl
      << "会员代码, "
      << "ParticipantID, " << data.ParticipantID << endl
      << "密码, "
      << "Password, " << data.Password << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "本地报单编号, "
      << "OrderLocalID, " << data.OrderLocalID << endl
      << "交易所交易员连接状态, "
      << "TraderConnectStatus, " << data.TraderConnectStatus << endl
      << "发出连接请求的日期, "
      << "ConnectRequestDate, " << data.ConnectRequestDate << endl
      << "发出连接请求的时间, "
      << "ConnectRequestTime, " << data.ConnectRequestTime << endl
      << "上次报告日期, "
      << "LastReportDate, " << data.LastReportDate << endl
      << "上次报告时间, "
      << "LastReportTime, " << data.LastReportTime << endl
      << "完成连接日期, "
      << "ConnectDate, " << data.ConnectDate << endl
      << "完成连接时间, "
      << "ConnectTime, " << data.ConnectTime << endl
      << "启动日期, "
      << "StartDate, " << data.StartDate << endl
      << "启动时间, "
      << "StartTime, " << data.StartTime << endl
      << "交易日, "
      << "TradingDay, " << data.TradingDay << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl;
  return out;
}

/* 投资者结算结果 */
ostream &operator<<(ostream &out, CThostFtdcSettlementInfoField &data) {
  out << "CThostFtdcSettlementInfoField" << endl
      << "交易日, "
      << "TradingDay, " << data.TradingDay << endl
      << "结算编号, "
      << "SettlementID, " << data.SettlementID << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "序号, "
      << "SequenceNo, " << data.SequenceNo << endl
      << "消息正文, "
      << "Content, " << data.Content << endl;
  return out;
}

/* 合约保证金率调整 */
ostream &operator<<(ostream &out,
                    CThostFtdcInstrumentMarginRateAdjustField &data) {
  out << "CThostFtdcInstrumentMarginRateAdjustField" << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "投资者范围, "
      << "InvestorRange, " << data.InvestorRange << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "投机套保标志, "
      << "HedgeFlag, " << data.HedgeFlag << endl
      << "多头保证金率, "
      << "LongMarginRatioByMoney, " << data.LongMarginRatioByMoney << endl
      << "多头保证金费, "
      << "LongMarginRatioByVolume, " << data.LongMarginRatioByVolume << endl
      << "空头保证金率, "
      << "ShortMarginRatioByMoney, " << data.ShortMarginRatioByMoney << endl
      << "空头保证金费, "
      << "ShortMarginRatioByVolume, " << data.ShortMarginRatioByVolume << endl
      << "是否相对交易所收取, "
      << "IsRelative, " << data.IsRelative << endl;
  return out;
}

/* 交易所保证金率 */
ostream &operator<<(ostream &out, CThostFtdcExchangeMarginRateField &data) {
  out << "CThostFtdcExchangeMarginRateField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "投机套保标志, "
      << "HedgeFlag, " << data.HedgeFlag << endl
      << "多头保证金率, "
      << "LongMarginRatioByMoney, " << data.LongMarginRatioByMoney << endl
      << "多头保证金费, "
      << "LongMarginRatioByVolume, " << data.LongMarginRatioByVolume << endl
      << "空头保证金率, "
      << "ShortMarginRatioByMoney, " << data.ShortMarginRatioByMoney << endl
      << "空头保证金费, "
      << "ShortMarginRatioByVolume, " << data.ShortMarginRatioByVolume << endl;
  return out;
}

/* 交易所保证金率调整 */
ostream &operator<<(ostream &out,
                    CThostFtdcExchangeMarginRateAdjustField &data) {
  out << "CThostFtdcExchangeMarginRateAdjustField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "投机套保标志, "
      << "HedgeFlag, " << data.HedgeFlag << endl
      << "跟随交易所投资者多头保证金率, "
      << "LongMarginRatioByMoney, " << data.LongMarginRatioByMoney << endl
      << "跟随交易所投资者多头保证金费, "
      << "LongMarginRatioByVolume, " << data.LongMarginRatioByVolume << endl
      << "跟随交易所投资者空头保证金率, "
      << "ShortMarginRatioByMoney, " << data.ShortMarginRatioByMoney << endl
      << "跟随交易所投资者空头保证金费, "
      << "ShortMarginRatioByVolume, " << data.ShortMarginRatioByVolume << endl
      << "交易所多头保证金率, "
      << "ExchLongMarginRatioByMoney, " << data.ExchLongMarginRatioByMoney
      << endl
      << "交易所多头保证金费, "
      << "ExchLongMarginRatioByVolume, " << data.ExchLongMarginRatioByVolume
      << endl
      << "交易所空头保证金率, "
      << "ExchShortMarginRatioByMoney, " << data.ExchShortMarginRatioByMoney
      << endl
      << "交易所空头保证金费, "
      << "ExchShortMarginRatioByVolume, " << data.ExchShortMarginRatioByVolume
      << endl
      << "不跟随交易所投资者多头保证金率, "
      << "NoLongMarginRatioByMoney, " << data.NoLongMarginRatioByMoney << endl
      << "不跟随交易所投资者多头保证金费, "
      << "NoLongMarginRatioByVolume, " << data.NoLongMarginRatioByVolume << endl
      << "不跟随交易所投资者空头保证金率, "
      << "NoShortMarginRatioByMoney, " << data.NoShortMarginRatioByMoney << endl
      << "不跟随交易所投资者空头保证金费, "
      << "NoShortMarginRatioByVolume, " << data.NoShortMarginRatioByVolume
      << endl;
  return out;
}

/* 结算引用 */
ostream &operator<<(ostream &out, CThostFtdcSettlementRefField &data) {
  out << "CThostFtdcSettlementRefField" << endl
      << "交易日, "
      << "TradingDay, " << data.TradingDay << endl
      << "结算编号, "
      << "SettlementID, " << data.SettlementID << endl;
  return out;
}

/* 当前时间 */
ostream &operator<<(ostream &out, CThostFtdcCurrentTimeField &data) {
  out << "CThostFtdcCurrentTimeField" << endl
      << "当前日期, "
      << "CurrDate, " << data.CurrDate << endl
      << "当前时间, "
      << "CurrTime, " << data.CurrTime << endl
      << "当前时间（毫秒）, "
      << "CurrMillisec, " << data.CurrMillisec << endl;
  return out;
}

/* 通讯阶段 */
ostream &operator<<(ostream &out, CThostFtdcCommPhaseField &data) {
  out << "CThostFtdcCommPhaseField" << endl
      << "交易日, "
      << "TradingDay, " << data.TradingDay << endl
      << "通讯时段编号, "
      << "CommPhaseNo, " << data.CommPhaseNo << endl;
  return out;
}

/* 登录信息 */
ostream &operator<<(ostream &out, CThostFtdcLoginInfoField &data) {
  out << "CThostFtdcLoginInfoField" << endl
      << "前置编号, "
      << "FrontID, " << data.FrontID << endl
      << "会话编号, "
      << "SessionID, " << data.SessionID << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "登录日期, "
      << "LoginDate, " << data.LoginDate << endl
      << "登录时间, "
      << "LoginTime, " << data.LoginTime << endl
      << "IP地址, "
      << "IPAddress, " << data.IPAddress << endl
      << "用户端产品信息, "
      << "UserProductInfo, " << data.UserProductInfo << endl
      << "接口端产品信息, "
      << "InterfaceProductInfo, " << data.InterfaceProductInfo << endl
      << "协议信息, "
      << "ProtocolInfo, " << data.ProtocolInfo << endl
      << "系统名称, "
      << "SystemName, " << data.SystemName << endl
      << "密码, "
      << "Password, " << data.Password << endl
      << "最大报单引用, "
      << "MaxOrderRef, " << data.MaxOrderRef << endl
      << "上期所时间, "
      << "SHFETime, " << data.SHFETime << endl
      << "大商所时间, "
      << "DCETime, " << data.DCETime << endl
      << "郑商所时间, "
      << "CZCETime, " << data.CZCETime << endl
      << "中金所时间, "
      << "FFEXTime, " << data.FFEXTime << endl
      << "Mac地址, "
      << "MacAddress, " << data.MacAddress << endl
      << "动态密码, "
      << "OneTimePassword, " << data.OneTimePassword << endl;
  return out;
}

/* 登录信息 */
ostream &operator<<(ostream &out, CThostFtdcLogoutAllField &data) {
  out << "CThostFtdcLogoutAllField" << endl
      << "前置编号, "
      << "FrontID, " << data.FrontID << endl
      << "会话编号, "
      << "SessionID, " << data.SessionID << endl
      << "系统名称, "
      << "SystemName, " << data.SystemName << endl;
  return out;
}

/* 前置状态 */
ostream &operator<<(ostream &out, CThostFtdcFrontStatusField &data) {
  out << "CThostFtdcFrontStatusField" << endl
      << "前置编号, "
      << "FrontID, " << data.FrontID << endl
      << "上次报告日期, "
      << "LastReportDate, " << data.LastReportDate << endl
      << "上次报告时间, "
      << "LastReportTime, " << data.LastReportTime << endl
      << "是否活跃, "
      << "IsActive, " << data.IsActive << endl;
  return out;
}

/* 用户口令变更 */
ostream &operator<<(ostream &out, CThostFtdcUserPasswordUpdateField &data) {
  out << "CThostFtdcUserPasswordUpdateField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "原来的口令, "
      << "OldPassword, " << data.OldPassword << endl
      << "新的口令, "
      << "NewPassword, " << data.NewPassword << endl;
  return out;
}

/* 输入报单 */
ostream &operator<<(ostream &out, CThostFtdcInputOrderField &data) {
  out << "CThostFtdcInputOrderField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "报单引用, "
      << "OrderRef, " << data.OrderRef << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "报单价格条件, "
      << "OrderPriceType, " << data.OrderPriceType << endl
      << "买卖方向, "
      << "Direction, " << data.Direction << endl
      << "组合开平标志, "
      << "CombOffsetFlag, " << data.CombOffsetFlag << endl
      << "组合投机套保标志, "
      << "CombHedgeFlag, " << data.CombHedgeFlag << endl
      << "价格, "
      << "LimitPrice, " << data.LimitPrice << endl
      << "数量, "
      << "VolumeTotalOriginal, " << data.VolumeTotalOriginal << endl
      << "有效期类型, "
      << "TimeCondition, " << data.TimeCondition << endl
      << "GTD日期, "
      << "GTDDate, " << data.GTDDate << endl
      << "成交量类型, "
      << "VolumeCondition, " << data.VolumeCondition << endl
      << "最小成交量, "
      << "MinVolume, " << data.MinVolume << endl
      << "触发条件, "
      << "ContingentCondition, " << data.ContingentCondition << endl
      << "止损价, "
      << "StopPrice, " << data.StopPrice << endl
      << "强平原因, "
      << "ForceCloseReason, " << data.ForceCloseReason << endl
      << "自动挂起标志, "
      << "IsAutoSuspend, " << data.IsAutoSuspend << endl
      << "业务单元, "
      << "BusinessUnit, " << data.BusinessUnit << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "用户强评标志, "
      << "UserForceClose, " << data.UserForceClose << endl;
  return out;
}

/* 报单 */
ostream &operator<<(ostream &out, CThostFtdcOrderField &data) {
  out << "CThostFtdcOrderField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "报单引用, "
      << "OrderRef, " << data.OrderRef << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "报单价格条件, "
      << "OrderPriceType, " << data.OrderPriceType << endl
      << "买卖方向, "
      << "Direction, " << data.Direction << endl
      << "组合开平标志, "
      << "CombOffsetFlag, " << data.CombOffsetFlag << endl
      << "组合投机套保标志, "
      << "CombHedgeFlag, " << data.CombHedgeFlag << endl
      << "价格, "
      << "LimitPrice, " << data.LimitPrice << endl
      << "数量, "
      << "VolumeTotalOriginal, " << data.VolumeTotalOriginal << endl
      << "有效期类型, "
      << "TimeCondition, " << data.TimeCondition << endl
      << "GTD日期, "
      << "GTDDate, " << data.GTDDate << endl
      << "成交量类型, "
      << "VolumeCondition, " << data.VolumeCondition << endl
      << "最小成交量, "
      << "MinVolume, " << data.MinVolume << endl
      << "触发条件, "
      << "ContingentCondition, " << data.ContingentCondition << endl
      << "止损价, "
      << "StopPrice, " << data.StopPrice << endl
      << "强平原因, "
      << "ForceCloseReason, " << data.ForceCloseReason << endl
      << "自动挂起标志, "
      << "IsAutoSuspend, " << data.IsAutoSuspend << endl
      << "业务单元, "
      << "BusinessUnit, " << data.BusinessUnit << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "本地报单编号, "
      << "OrderLocalID, " << data.OrderLocalID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "会员代码, "
      << "ParticipantID, " << data.ParticipantID << endl
      << "客户代码, "
      << "ClientID, " << data.ClientID << endl
      << "合约在交易所的代码, "
      << "ExchangeInstID, " << data.ExchangeInstID << endl
      << "交易所交易员代码, "
      << "TraderID, " << data.TraderID << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "报单提交状态, "
      << "OrderSubmitStatus, " << data.OrderSubmitStatus << endl
      << "报单提示序号, "
      << "NotifySequence, " << data.NotifySequence << endl
      << "交易日, "
      << "TradingDay, " << data.TradingDay << endl
      << "结算编号, "
      << "SettlementID, " << data.SettlementID << endl
      << "报单编号, "
      << "OrderSysID, " << data.OrderSysID << endl
      << "报单来源, "
      << "OrderSource, " << data.OrderSource << endl
      << "报单状态, "
      << "OrderStatus, " << data.OrderStatus << endl
      << "报单类型, "
      << "OrderType, " << data.OrderType << endl
      << "今成交数量, "
      << "VolumeTraded, " << data.VolumeTraded << endl
      << "剩余数量, "
      << "VolumeTotal, " << data.VolumeTotal << endl
      << "报单日期, "
      << "InsertDate, " << data.InsertDate << endl
      << "委托时间, "
      << "InsertTime, " << data.InsertTime << endl
      << "激活时间, "
      << "ActiveTime, " << data.ActiveTime << endl
      << "挂起时间, "
      << "SuspendTime, " << data.SuspendTime << endl
      << "最后修改时间, "
      << "UpdateTime, " << data.UpdateTime << endl
      << "撤销时间, "
      << "CancelTime, " << data.CancelTime << endl
      << "最后修改交易所交易员代码, "
      << "ActiveTraderID, " << data.ActiveTraderID << endl
      << "结算会员编号, "
      << "ClearingPartID, " << data.ClearingPartID << endl
      << "序号, "
      << "SequenceNo, " << data.SequenceNo << endl
      << "前置编号, "
      << "FrontID, " << data.FrontID << endl
      << "会话编号, "
      << "SessionID, " << data.SessionID << endl
      << "用户端产品信息, "
      << "UserProductInfo, " << data.UserProductInfo << endl
      << "状态信息, "
      << "StatusMsg, " << data.StatusMsg << endl
      << "用户强评标志, "
      << "UserForceClose, " << data.UserForceClose << endl
      << "操作用户代码, "
      << "ActiveUserID, " << data.ActiveUserID << endl
      << "经纪公司报单编号, "
      << "BrokerOrderSeq, " << data.BrokerOrderSeq << endl
      << "相关报单, "
      << "RelativeOrderSysID, " << data.RelativeOrderSysID << endl;
  return out;
}

/* 交易所报单 */
ostream &operator<<(ostream &out, CThostFtdcExchangeOrderField &data) {
  out << "CThostFtdcExchangeOrderField" << endl
      << "报单价格条件, "
      << "OrderPriceType, " << data.OrderPriceType << endl
      << "买卖方向, "
      << "Direction, " << data.Direction << endl
      << "组合开平标志, "
      << "CombOffsetFlag, " << data.CombOffsetFlag << endl
      << "组合投机套保标志, "
      << "CombHedgeFlag, " << data.CombHedgeFlag << endl
      << "价格, "
      << "LimitPrice, " << data.LimitPrice << endl
      << "数量, "
      << "VolumeTotalOriginal, " << data.VolumeTotalOriginal << endl
      << "有效期类型, "
      << "TimeCondition, " << data.TimeCondition << endl
      << "GTD日期, "
      << "GTDDate, " << data.GTDDate << endl
      << "成交量类型, "
      << "VolumeCondition, " << data.VolumeCondition << endl
      << "最小成交量, "
      << "MinVolume, " << data.MinVolume << endl
      << "触发条件, "
      << "ContingentCondition, " << data.ContingentCondition << endl
      << "止损价, "
      << "StopPrice, " << data.StopPrice << endl
      << "强平原因, "
      << "ForceCloseReason, " << data.ForceCloseReason << endl
      << "自动挂起标志, "
      << "IsAutoSuspend, " << data.IsAutoSuspend << endl
      << "业务单元, "
      << "BusinessUnit, " << data.BusinessUnit << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "本地报单编号, "
      << "OrderLocalID, " << data.OrderLocalID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "会员代码, "
      << "ParticipantID, " << data.ParticipantID << endl
      << "客户代码, "
      << "ClientID, " << data.ClientID << endl
      << "合约在交易所的代码, "
      << "ExchangeInstID, " << data.ExchangeInstID << endl
      << "交易所交易员代码, "
      << "TraderID, " << data.TraderID << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "报单提交状态, "
      << "OrderSubmitStatus, " << data.OrderSubmitStatus << endl
      << "报单提示序号, "
      << "NotifySequence, " << data.NotifySequence << endl
      << "交易日, "
      << "TradingDay, " << data.TradingDay << endl
      << "结算编号, "
      << "SettlementID, " << data.SettlementID << endl
      << "报单编号, "
      << "OrderSysID, " << data.OrderSysID << endl
      << "报单来源, "
      << "OrderSource, " << data.OrderSource << endl
      << "报单状态, "
      << "OrderStatus, " << data.OrderStatus << endl
      << "报单类型, "
      << "OrderType, " << data.OrderType << endl
      << "今成交数量, "
      << "VolumeTraded, " << data.VolumeTraded << endl
      << "剩余数量, "
      << "VolumeTotal, " << data.VolumeTotal << endl
      << "报单日期, "
      << "InsertDate, " << data.InsertDate << endl
      << "委托时间, "
      << "InsertTime, " << data.InsertTime << endl
      << "激活时间, "
      << "ActiveTime, " << data.ActiveTime << endl
      << "挂起时间, "
      << "SuspendTime, " << data.SuspendTime << endl
      << "最后修改时间, "
      << "UpdateTime, " << data.UpdateTime << endl
      << "撤销时间, "
      << "CancelTime, " << data.CancelTime << endl
      << "最后修改交易所交易员代码, "
      << "ActiveTraderID, " << data.ActiveTraderID << endl
      << "结算会员编号, "
      << "ClearingPartID, " << data.ClearingPartID << endl
      << "序号, "
      << "SequenceNo, " << data.SequenceNo << endl;
  return out;
}

/* 交易所报单插入失败 */
ostream &operator<<(ostream &out,
                    CThostFtdcExchangeOrderInsertErrorField &data) {
  out << "CThostFtdcExchangeOrderInsertErrorField" << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "会员代码, "
      << "ParticipantID, " << data.ParticipantID << endl
      << "交易所交易员代码, "
      << "TraderID, " << data.TraderID << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "本地报单编号, "
      << "OrderLocalID, " << data.OrderLocalID << endl
      << "错误代码, "
      << "ErrorID, " << data.ErrorID << endl
      << "错误信息, "
      << "ErrorMsg, " << data.ErrorMsg << endl;
  return out;
}

/* 输入报单操作 */
ostream &operator<<(ostream &out, CThostFtdcInputOrderActionField &data) {
  out << "CThostFtdcInputOrderActionField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "报单操作引用, "
      << "OrderActionRef, " << data.OrderActionRef << endl
      << "报单引用, "
      << "OrderRef, " << data.OrderRef << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "前置编号, "
      << "FrontID, " << data.FrontID << endl
      << "会话编号, "
      << "SessionID, " << data.SessionID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "报单编号, "
      << "OrderSysID, " << data.OrderSysID << endl
      << "操作标志, "
      << "ActionFlag, " << data.ActionFlag << endl
      << "价格, "
      << "LimitPrice, " << data.LimitPrice << endl
      << "数量变化, "
      << "VolumeChange, " << data.VolumeChange << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl;
  return out;
}

/* 报单操作 */
ostream &operator<<(ostream &out, CThostFtdcOrderActionField &data) {
  out << "CThostFtdcOrderActionField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "报单操作引用, "
      << "OrderActionRef, " << data.OrderActionRef << endl
      << "报单引用, "
      << "OrderRef, " << data.OrderRef << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "前置编号, "
      << "FrontID, " << data.FrontID << endl
      << "会话编号, "
      << "SessionID, " << data.SessionID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "报单编号, "
      << "OrderSysID, " << data.OrderSysID << endl
      << "操作标志, "
      << "ActionFlag, " << data.ActionFlag << endl
      << "价格, "
      << "LimitPrice, " << data.LimitPrice << endl
      << "数量变化, "
      << "VolumeChange, " << data.VolumeChange << endl
      << "操作日期, "
      << "ActionDate, " << data.ActionDate << endl
      << "操作时间, "
      << "ActionTime, " << data.ActionTime << endl
      << "交易所交易员代码, "
      << "TraderID, " << data.TraderID << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "本地报单编号, "
      << "OrderLocalID, " << data.OrderLocalID << endl
      << "操作本地编号, "
      << "ActionLocalID, " << data.ActionLocalID << endl
      << "会员代码, "
      << "ParticipantID, " << data.ParticipantID << endl
      << "客户代码, "
      << "ClientID, " << data.ClientID << endl
      << "业务单元, "
      << "BusinessUnit, " << data.BusinessUnit << endl
      << "报单操作状态, "
      << "OrderActionStatus, " << data.OrderActionStatus << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "状态信息, "
      << "StatusMsg, " << data.StatusMsg << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl;
  return out;
}

/* 交易所报单操作 */
ostream &operator<<(ostream &out, CThostFtdcExchangeOrderActionField &data) {
  out << "CThostFtdcExchangeOrderActionField" << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "报单编号, "
      << "OrderSysID, " << data.OrderSysID << endl
      << "操作标志, "
      << "ActionFlag, " << data.ActionFlag << endl
      << "价格, "
      << "LimitPrice, " << data.LimitPrice << endl
      << "数量变化, "
      << "VolumeChange, " << data.VolumeChange << endl
      << "操作日期, "
      << "ActionDate, " << data.ActionDate << endl
      << "操作时间, "
      << "ActionTime, " << data.ActionTime << endl
      << "交易所交易员代码, "
      << "TraderID, " << data.TraderID << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "本地报单编号, "
      << "OrderLocalID, " << data.OrderLocalID << endl
      << "操作本地编号, "
      << "ActionLocalID, " << data.ActionLocalID << endl
      << "会员代码, "
      << "ParticipantID, " << data.ParticipantID << endl
      << "客户代码, "
      << "ClientID, " << data.ClientID << endl
      << "业务单元, "
      << "BusinessUnit, " << data.BusinessUnit << endl
      << "报单操作状态, "
      << "OrderActionStatus, " << data.OrderActionStatus << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl;
  return out;
}

/* 交易所报单操作失败 */
ostream &operator<<(ostream &out,
                    CThostFtdcExchangeOrderActionErrorField &data) {
  out << "CThostFtdcExchangeOrderActionErrorField" << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "报单编号, "
      << "OrderSysID, " << data.OrderSysID << endl
      << "交易所交易员代码, "
      << "TraderID, " << data.TraderID << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "本地报单编号, "
      << "OrderLocalID, " << data.OrderLocalID << endl
      << "操作本地编号, "
      << "ActionLocalID, " << data.ActionLocalID << endl
      << "错误代码, "
      << "ErrorID, " << data.ErrorID << endl
      << "错误信息, "
      << "ErrorMsg, " << data.ErrorMsg << endl;
  return out;
}

/* 交易所成交 */
ostream &operator<<(ostream &out, CThostFtdcExchangeTradeField &data) {
  out << "CThostFtdcExchangeTradeField" << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "成交编号, "
      << "TradeID, " << data.TradeID << endl
      << "买卖方向, "
      << "Direction, " << data.Direction << endl
      << "报单编号, "
      << "OrderSysID, " << data.OrderSysID << endl
      << "会员代码, "
      << "ParticipantID, " << data.ParticipantID << endl
      << "客户代码, "
      << "ClientID, " << data.ClientID << endl
      << "交易角色, "
      << "TradingRole, " << data.TradingRole << endl
      << "合约在交易所的代码, "
      << "ExchangeInstID, " << data.ExchangeInstID << endl
      << "开平标志, "
      << "OffsetFlag, " << data.OffsetFlag << endl
      << "投机套保标志, "
      << "HedgeFlag, " << data.HedgeFlag << endl
      << "价格, "
      << "Price, " << data.Price << endl
      << "数量, "
      << "Volume, " << data.Volume << endl
      << "成交时期, "
      << "TradeDate, " << data.TradeDate << endl
      << "成交时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "成交类型, "
      << "TradeType, " << data.TradeType << endl
      << "成交价来源, "
      << "PriceSource, " << data.PriceSource << endl
      << "交易所交易员代码, "
      << "TraderID, " << data.TraderID << endl
      << "本地报单编号, "
      << "OrderLocalID, " << data.OrderLocalID << endl
      << "结算会员编号, "
      << "ClearingPartID, " << data.ClearingPartID << endl
      << "业务单元, "
      << "BusinessUnit, " << data.BusinessUnit << endl
      << "序号, "
      << "SequenceNo, " << data.SequenceNo << endl;
  return out;
}

/* 成交 */
ostream &operator<<(ostream &out, CThostFtdcTradeField &data) {
  out << "CThostFtdcTradeField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "报单引用, "
      << "OrderRef, " << data.OrderRef << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "成交编号, "
      << "TradeID, " << data.TradeID << endl
      << "买卖方向, "
      << "Direction, " << data.Direction << endl
      << "报单编号, "
      << "OrderSysID, " << data.OrderSysID << endl
      << "会员代码, "
      << "ParticipantID, " << data.ParticipantID << endl
      << "客户代码, "
      << "ClientID, " << data.ClientID << endl
      << "交易角色, "
      << "TradingRole, " << data.TradingRole << endl
      << "合约在交易所的代码, "
      << "ExchangeInstID, " << data.ExchangeInstID << endl
      << "开平标志, "
      << "OffsetFlag, " << data.OffsetFlag << endl
      << "投机套保标志, "
      << "HedgeFlag, " << data.HedgeFlag << endl
      << "价格, "
      << "Price, " << data.Price << endl
      << "数量, "
      << "Volume, " << data.Volume << endl
      << "成交时期, "
      << "TradeDate, " << data.TradeDate << endl
      << "成交时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "成交类型, "
      << "TradeType, " << data.TradeType << endl
      << "成交价来源, "
      << "PriceSource, " << data.PriceSource << endl
      << "交易所交易员代码, "
      << "TraderID, " << data.TraderID << endl
      << "本地报单编号, "
      << "OrderLocalID, " << data.OrderLocalID << endl
      << "结算会员编号, "
      << "ClearingPartID, " << data.ClearingPartID << endl
      << "业务单元, "
      << "BusinessUnit, " << data.BusinessUnit << endl
      << "序号, "
      << "SequenceNo, " << data.SequenceNo << endl
      << "交易日, "
      << "TradingDay, " << data.TradingDay << endl
      << "结算编号, "
      << "SettlementID, " << data.SettlementID << endl
      << "经纪公司报单编号, "
      << "BrokerOrderSeq, " << data.BrokerOrderSeq << endl;
  return out;
}

/* 用户会话 */
ostream &operator<<(ostream &out, CThostFtdcUserSessionField &data) {
  out << "CThostFtdcUserSessionField" << endl
      << "前置编号, "
      << "FrontID, " << data.FrontID << endl
      << "会话编号, "
      << "SessionID, " << data.SessionID << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "登录日期, "
      << "LoginDate, " << data.LoginDate << endl
      << "登录时间, "
      << "LoginTime, " << data.LoginTime << endl
      << "IP地址, "
      << "IPAddress, " << data.IPAddress << endl
      << "用户端产品信息, "
      << "UserProductInfo, " << data.UserProductInfo << endl
      << "接口端产品信息, "
      << "InterfaceProductInfo, " << data.InterfaceProductInfo << endl
      << "协议信息, "
      << "ProtocolInfo, " << data.ProtocolInfo << endl
      << "Mac地址, "
      << "MacAddress, " << data.MacAddress << endl;
  return out;
}

/* 查询最大报单数量 */
ostream &operator<<(ostream &out, CThostFtdcQueryMaxOrderVolumeField &data) {
  out << "CThostFtdcQueryMaxOrderVolumeField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "买卖方向, "
      << "Direction, " << data.Direction << endl
      << "开平标志, "
      << "OffsetFlag, " << data.OffsetFlag << endl
      << "投机套保标志, "
      << "HedgeFlag, " << data.HedgeFlag << endl
      << "最大允许报单数量, "
      << "MaxVolume, " << data.MaxVolume << endl;
  return out;
}

/* 投资者结算结果确认信息 */
ostream &operator<<(ostream &out, CThostFtdcSettlementInfoConfirmField &data) {
  out << "CThostFtdcSettlementInfoConfirmField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "确认日期, "
      << "ConfirmDate, " << data.ConfirmDate << endl
      << "确认时间, "
      << "ConfirmTime, " << data.ConfirmTime << endl;
  return out;
}

/* 出入金同步 */
ostream &operator<<(ostream &out, CThostFtdcSyncDepositField &data) {
  out << "CThostFtdcSyncDepositField" << endl
      << "出入金流水号, "
      << "DepositSeqNo, " << data.DepositSeqNo << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "入金金额, "
      << "Deposit, " << data.Deposit << endl
      << "是否强制进行, "
      << "IsForce, " << data.IsForce << endl;
  return out;
}

/* 经纪公司同步 */
ostream &operator<<(ostream &out, CThostFtdcBrokerSyncField &data) {
  out << "CThostFtdcBrokerSyncField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl;
  return out;
}

/* 正在同步中的投资者 */
ostream &operator<<(ostream &out, CThostFtdcSyncingInvestorField &data) {
  out << "CThostFtdcSyncingInvestorField" << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者分组代码, "
      << "InvestorGroupID, " << data.InvestorGroupID << endl
      << "投资者名称, "
      << "InvestorName, " << data.InvestorName << endl
      << "证件类型, "
      << "IdentifiedCardType, " << data.IdentifiedCardType << endl
      << "证件号码, "
      << "IdentifiedCardNo, " << data.IdentifiedCardNo << endl
      << "是否活跃, "
      << "IsActive, " << data.IsActive << endl
      << "联系电话, "
      << "Telephone, " << data.Telephone << endl
      << "通讯地址, "
      << "Address, " << data.Address << endl
      << "开户日期, "
      << "OpenDate, " << data.OpenDate << endl
      << "手机, "
      << "Mobile, " << data.Mobile << endl;
  return out;
}

/* 正在同步中的交易代码 */
ostream &operator<<(ostream &out, CThostFtdcSyncingTradingCodeField &data) {
  out << "CThostFtdcSyncingTradingCodeField" << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "客户代码, "
      << "ClientID, " << data.ClientID << endl
      << "是否活跃, "
      << "IsActive, " << data.IsActive << endl
      << "交易编码类型, "
      << "ClientIDType, " << data.ClientIDType << endl;
  return out;
}

/* 正在同步中的投资者分组 */
ostream &operator<<(ostream &out, CThostFtdcSyncingInvestorGroupField &data) {
  out << "CThostFtdcSyncingInvestorGroupField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者分组代码, "
      << "InvestorGroupID, " << data.InvestorGroupID << endl
      << "投资者分组名称, "
      << "InvestorGroupName, " << data.InvestorGroupName << endl;
  return out;
}

/* 正在同步中的交易账号 */
ostream &operator<<(ostream &out, CThostFtdcSyncingTradingAccountField &data) {
  out << "CThostFtdcSyncingTradingAccountField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者帐号, "
      << "AccountID, " << data.AccountID << endl
      << "上次质押金额, "
      << "PreMortgage, " << data.PreMortgage << endl
      << "上次信用额度, "
      << "PreCredit, " << data.PreCredit << endl
      << "上次存款额, "
      << "PreDeposit, " << data.PreDeposit << endl
      << "上次结算准备金, "
      << "PreBalance, " << data.PreBalance << endl
      << "上次占用的保证金, "
      << "PreMargin, " << data.PreMargin << endl
      << "利息基数, "
      << "InterestBase, " << data.InterestBase << endl
      << "利息收入, "
      << "Interest, " << data.Interest << endl
      << "入金金额, "
      << "Deposit, " << data.Deposit << endl
      << "出金金额, "
      << "Withdraw, " << data.Withdraw << endl
      << "冻结的保证金, "
      << "FrozenMargin, " << data.FrozenMargin << endl
      << "冻结的资金, "
      << "FrozenCash, " << data.FrozenCash << endl
      << "冻结的手续费, "
      << "FrozenCommission, " << data.FrozenCommission << endl
      << "当前保证金总额, "
      << "CurrMargin, " << data.CurrMargin << endl
      << "资金差额, "
      << "CashIn, " << data.CashIn << endl
      << "手续费, "
      << "Commission, " << data.Commission << endl
      << "平仓盈亏, "
      << "CloseProfit, " << data.CloseProfit << endl
      << "持仓盈亏, "
      << "PositionProfit, " << data.PositionProfit << endl
      << "期货结算准备金, "
      << "Balance, " << data.Balance << endl
      << "可用资金, "
      << "Available, " << data.Available << endl
      << "可取资金, "
      << "WithdrawQuota, " << data.WithdrawQuota << endl
      << "基本准备金, "
      << "Reserve, " << data.Reserve << endl
      << "交易日, "
      << "TradingDay, " << data.TradingDay << endl
      << "结算编号, "
      << "SettlementID, " << data.SettlementID << endl
      << "信用额度, "
      << "Credit, " << data.Credit << endl
      << "质押金额, "
      << "Mortgage, " << data.Mortgage << endl
      << "交易所保证金, "
      << "ExchangeMargin, " << data.ExchangeMargin << endl
      << "投资者交割保证金, "
      << "DeliveryMargin, " << data.DeliveryMargin << endl
      << "交易所交割保证金, "
      << "ExchangeDeliveryMargin, " << data.ExchangeDeliveryMargin << endl;
  return out;
}

/* 正在同步中的投资者持仓 */
ostream &operator<<(ostream &out,
                    CThostFtdcSyncingInvestorPositionField &data) {
  out << "CThostFtdcSyncingInvestorPositionField" << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "持仓多空方向, "
      << "PosiDirection, " << data.PosiDirection << endl
      << "投机套保标志, "
      << "HedgeFlag, " << data.HedgeFlag << endl
      << "持仓日期, "
      << "PositionDate, " << data.PositionDate << endl
      << "上日持仓, "
      << "YdPosition, " << data.YdPosition << endl
      << "今日持仓, "
      << "Position, " << data.Position << endl
      << "多头冻结, "
      << "LongFrozen, " << data.LongFrozen << endl
      << "空头冻结, "
      << "ShortFrozen, " << data.ShortFrozen << endl
      << "开仓冻结金额, "
      << "LongFrozenAmount, " << data.LongFrozenAmount << endl
      << "开仓冻结金额, "
      << "ShortFrozenAmount, " << data.ShortFrozenAmount << endl
      << "开仓量, "
      << "OpenVolume, " << data.OpenVolume << endl
      << "平仓量, "
      << "CloseVolume, " << data.CloseVolume << endl
      << "开仓金额, "
      << "OpenAmount, " << data.OpenAmount << endl
      << "平仓金额, "
      << "CloseAmount, " << data.CloseAmount << endl
      << "持仓成本, "
      << "PositionCost, " << data.PositionCost << endl
      << "上次占用的保证金, "
      << "PreMargin, " << data.PreMargin << endl
      << "占用的保证金, "
      << "UseMargin, " << data.UseMargin << endl
      << "冻结的保证金, "
      << "FrozenMargin, " << data.FrozenMargin << endl
      << "冻结的资金, "
      << "FrozenCash, " << data.FrozenCash << endl
      << "冻结的手续费, "
      << "FrozenCommission, " << data.FrozenCommission << endl
      << "资金差额, "
      << "CashIn, " << data.CashIn << endl
      << "手续费, "
      << "Commission, " << data.Commission << endl
      << "平仓盈亏, "
      << "CloseProfit, " << data.CloseProfit << endl
      << "持仓盈亏, "
      << "PositionProfit, " << data.PositionProfit << endl
      << "上次结算价, "
      << "PreSettlementPrice, " << data.PreSettlementPrice << endl
      << "本次结算价, "
      << "SettlementPrice, " << data.SettlementPrice << endl
      << "交易日, "
      << "TradingDay, " << data.TradingDay << endl
      << "结算编号, "
      << "SettlementID, " << data.SettlementID << endl
      << "开仓成本, "
      << "OpenCost, " << data.OpenCost << endl
      << "交易所保证金, "
      << "ExchangeMargin, " << data.ExchangeMargin << endl
      << "组合成交形成的持仓, "
      << "CombPosition, " << data.CombPosition << endl
      << "组合多头冻结, "
      << "CombLongFrozen, " << data.CombLongFrozen << endl
      << "组合空头冻结, "
      << "CombShortFrozen, " << data.CombShortFrozen << endl
      << "逐日盯市平仓盈亏, "
      << "CloseProfitByDate, " << data.CloseProfitByDate << endl
      << "逐笔对冲平仓盈亏, "
      << "CloseProfitByTrade, " << data.CloseProfitByTrade << endl
      << "今日持仓, "
      << "TodayPosition, " << data.TodayPosition << endl
      << "保证金率, "
      << "MarginRateByMoney, " << data.MarginRateByMoney << endl
      << "保证金率(按手数), "
      << "MarginRateByVolume, " << data.MarginRateByVolume << endl;
  return out;
}

/* 正在同步中的合约保证金率 */
ostream &operator<<(ostream &out,
                    CThostFtdcSyncingInstrumentMarginRateField &data) {
  out << "CThostFtdcSyncingInstrumentMarginRateField" << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "投资者范围, "
      << "InvestorRange, " << data.InvestorRange << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "投机套保标志, "
      << "HedgeFlag, " << data.HedgeFlag << endl
      << "多头保证金率, "
      << "LongMarginRatioByMoney, " << data.LongMarginRatioByMoney << endl
      << "多头保证金费, "
      << "LongMarginRatioByVolume, " << data.LongMarginRatioByVolume << endl
      << "空头保证金率, "
      << "ShortMarginRatioByMoney, " << data.ShortMarginRatioByMoney << endl
      << "空头保证金费, "
      << "ShortMarginRatioByVolume, " << data.ShortMarginRatioByVolume << endl
      << "是否相对交易所收取, "
      << "IsRelative, " << data.IsRelative << endl;
  return out;
}

/* 正在同步中的合约手续费率 */
ostream &operator<<(ostream &out,
                    CThostFtdcSyncingInstrumentCommissionRateField &data) {
  out << "CThostFtdcSyncingInstrumentCommissionRateField" << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "投资者范围, "
      << "InvestorRange, " << data.InvestorRange << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "开仓手续费率, "
      << "OpenRatioByMoney, " << data.OpenRatioByMoney << endl
      << "开仓手续费, "
      << "OpenRatioByVolume, " << data.OpenRatioByVolume << endl
      << "平仓手续费率, "
      << "CloseRatioByMoney, " << data.CloseRatioByMoney << endl
      << "平仓手续费, "
      << "CloseRatioByVolume, " << data.CloseRatioByVolume << endl
      << "平今手续费率, "
      << "CloseTodayRatioByMoney, " << data.CloseTodayRatioByMoney << endl
      << "平今手续费, "
      << "CloseTodayRatioByVolume, " << data.CloseTodayRatioByVolume << endl;
  return out;
}

/* 正在同步中的合约交易权限 */
ostream &operator<<(ostream &out,
                    CThostFtdcSyncingInstrumentTradingRightField &data) {
  out << "CThostFtdcSyncingInstrumentTradingRightField" << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "投资者范围, "
      << "InvestorRange, " << data.InvestorRange << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "交易权限, "
      << "TradingRight, " << data.TradingRight << endl;
  return out;
}

/* 查询报单 */
ostream &operator<<(ostream &out, CThostFtdcQryOrderField &data) {
  out << "CThostFtdcQryOrderField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "报单编号, "
      << "OrderSysID, " << data.OrderSysID << endl
      << "开始时间, "
      << "InsertTimeStart, " << data.InsertTimeStart << endl
      << "结束时间, "
      << "InsertTimeEnd, " << data.InsertTimeEnd << endl;
  return out;
}

/* 查询成交 */
ostream &operator<<(ostream &out, CThostFtdcQryTradeField &data) {
  out << "CThostFtdcQryTradeField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "成交编号, "
      << "TradeID, " << data.TradeID << endl
      << "开始时间, "
      << "TradeTimeStart, " << data.TradeTimeStart << endl
      << "结束时间, "
      << "TradeTimeEnd, " << data.TradeTimeEnd << endl;
  return out;
}

/* 查询投资者持仓 */
ostream &operator<<(ostream &out, CThostFtdcQryInvestorPositionField &data) {
  out << "CThostFtdcQryInvestorPositionField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl;
  return out;
}

/* 查询资金账户 */
ostream &operator<<(ostream &out, CThostFtdcQryTradingAccountField &data) {
  out << "CThostFtdcQryTradingAccountField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl;
  return out;
}

/* 查询投资者 */
ostream &operator<<(ostream &out, CThostFtdcQryInvestorField &data) {
  out << "CThostFtdcQryInvestorField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl;
  return out;
}

/* 查询交易编码 */
ostream &operator<<(ostream &out, CThostFtdcQryTradingCodeField &data) {
  out << "CThostFtdcQryTradingCodeField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "客户代码, "
      << "ClientID, " << data.ClientID << endl
      << "交易编码类型, "
      << "ClientIDType, " << data.ClientIDType << endl;
  return out;
}

/* 查询交易编码 */
ostream &operator<<(ostream &out, CThostFtdcQryInvestorGroupField &data) {
  out << "CThostFtdcQryInvestorGroupField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl;
  return out;
}

/* 查询交易编码 */
ostream &operator<<(ostream &out,
                    CThostFtdcQryInstrumentMarginRateField &data) {
  out << "CThostFtdcQryInstrumentMarginRateField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "投机套保标志, "
      << "HedgeFlag, " << data.HedgeFlag << endl;
  return out;
}

/* 查询交易编码 */
ostream &operator<<(ostream &out,
                    CThostFtdcQryInstrumentCommissionRateField &data) {
  out << "CThostFtdcQryInstrumentCommissionRateField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl;
  return out;
}

/* 查询交易编码 */
ostream &operator<<(ostream &out,
                    CThostFtdcQryInstrumentTradingRightField &data) {
  out << "CThostFtdcQryInstrumentTradingRightField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl;
  return out;
}

/* 查询经纪公司 */
ostream &operator<<(ostream &out, CThostFtdcQryBrokerField &data) {
  out << "CThostFtdcQryBrokerField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl;
  return out;
}

/* 查询交易员 */
ostream &operator<<(ostream &out, CThostFtdcQryTraderField &data) {
  out << "CThostFtdcQryTraderField" << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "会员代码, "
      << "ParticipantID, " << data.ParticipantID << endl
      << "交易所交易员代码, "
      << "TraderID, " << data.TraderID << endl;
  return out;
}

/* 查询经纪公司会员代码 */
ostream &operator<<(ostream &out, CThostFtdcQryPartBrokerField &data) {
  out << "CThostFtdcQryPartBrokerField" << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "会员代码, "
      << "ParticipantID, " << data.ParticipantID << endl;
  return out;
}

/* 查询管理用户功能权限 */
ostream &operator<<(ostream &out, CThostFtdcQrySuperUserFunctionField &data) {
  out << "CThostFtdcQrySuperUserFunctionField" << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl;
  return out;
}

/* 查询用户会话 */
ostream &operator<<(ostream &out, CThostFtdcQryUserSessionField &data) {
  out << "CThostFtdcQryUserSessionField" << endl
      << "前置编号, "
      << "FrontID, " << data.FrontID << endl
      << "会话编号, "
      << "SessionID, " << data.SessionID << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl;
  return out;
}

/* 查询前置状态 */
ostream &operator<<(ostream &out, CThostFtdcQryFrontStatusField &data) {
  out << "CThostFtdcQryFrontStatusField" << endl
      << "前置编号, "
      << "FrontID, " << data.FrontID << endl;
  return out;
}

/* 查询交易所报单 */
ostream &operator<<(ostream &out, CThostFtdcQryExchangeOrderField &data) {
  out << "CThostFtdcQryExchangeOrderField" << endl
      << "会员代码, "
      << "ParticipantID, " << data.ParticipantID << endl
      << "客户代码, "
      << "ClientID, " << data.ClientID << endl
      << "合约在交易所的代码, "
      << "ExchangeInstID, " << data.ExchangeInstID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "交易所交易员代码, "
      << "TraderID, " << data.TraderID << endl;
  return out;
}

/* 查询报单操作 */
ostream &operator<<(ostream &out, CThostFtdcQryOrderActionField &data) {
  out << "CThostFtdcQryOrderActionField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl;
  return out;
}

/* 查询交易所报单操作 */
ostream &operator<<(ostream &out, CThostFtdcQryExchangeOrderActionField &data) {
  out << "CThostFtdcQryExchangeOrderActionField" << endl
      << "会员代码, "
      << "ParticipantID, " << data.ParticipantID << endl
      << "客户代码, "
      << "ClientID, " << data.ClientID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "交易所交易员代码, "
      << "TraderID, " << data.TraderID << endl;
  return out;
}

/* 查询管理用户 */
ostream &operator<<(ostream &out, CThostFtdcQrySuperUserField &data) {
  out << "CThostFtdcQrySuperUserField" << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl;
  return out;
}

/* 查询交易所 */
ostream &operator<<(ostream &out, CThostFtdcQryExchangeField &data) {
  out << "CThostFtdcQryExchangeField" << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl;
  return out;
}

/* 查询产品 */
ostream &operator<<(ostream &out, CThostFtdcQryProductField &data) {
  out << "CThostFtdcQryProductField" << endl
      << "产品代码, "
      << "ProductID, " << data.ProductID << endl;
  return out;
}

/* 查询合约 */
ostream &operator<<(ostream &out, CThostFtdcQryInstrumentField &data) {
  out << "CThostFtdcQryInstrumentField" << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "合约在交易所的代码, "
      << "ExchangeInstID, " << data.ExchangeInstID << endl
      << "产品代码, "
      << "ProductID, " << data.ProductID << endl;
  return out;
}

/* 查询行情 */
ostream &operator<<(ostream &out, CThostFtdcQryDepthMarketDataField &data) {
  out << "CThostFtdcQryDepthMarketDataField" << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl;
  return out;
}

/* 查询经纪公司用户 */
ostream &operator<<(ostream &out, CThostFtdcQryBrokerUserField &data) {
  out << "CThostFtdcQryBrokerUserField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl;
  return out;
}

/* 查询经纪公司用户权限 */
ostream &operator<<(ostream &out, CThostFtdcQryBrokerUserFunctionField &data) {
  out << "CThostFtdcQryBrokerUserFunctionField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl;
  return out;
}

/* 查询交易员报盘机 */
ostream &operator<<(ostream &out, CThostFtdcQryTraderOfferField &data) {
  out << "CThostFtdcQryTraderOfferField" << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "会员代码, "
      << "ParticipantID, " << data.ParticipantID << endl
      << "交易所交易员代码, "
      << "TraderID, " << data.TraderID << endl;
  return out;
}

/* 查询出入金流水 */
ostream &operator<<(ostream &out, CThostFtdcQrySyncDepositField &data) {
  out << "CThostFtdcQrySyncDepositField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "出入金流水号, "
      << "DepositSeqNo, " << data.DepositSeqNo << endl;
  return out;
}

/* 查询投资者结算结果 */
ostream &operator<<(ostream &out, CThostFtdcQrySettlementInfoField &data) {
  out << "CThostFtdcQrySettlementInfoField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "交易日, "
      << "TradingDay, " << data.TradingDay << endl;
  return out;
}

/* 查询报单 */
ostream &operator<<(ostream &out, CThostFtdcQryHisOrderField &data) {
  out << "CThostFtdcQryHisOrderField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "报单编号, "
      << "OrderSysID, " << data.OrderSysID << endl
      << "开始时间, "
      << "InsertTimeStart, " << data.InsertTimeStart << endl
      << "结束时间, "
      << "InsertTimeEnd, " << data.InsertTimeEnd << endl
      << "交易日, "
      << "TradingDay, " << data.TradingDay << endl
      << "结算编号, "
      << "SettlementID, " << data.SettlementID << endl;
  return out;
}

/* 市场行情 */
ostream &operator<<(ostream &out, CThostFtdcMarketDataField &data) {
  out << "CThostFtdcMarketDataField" << endl
      << "交易日, "
      << "TradingDay, " << data.TradingDay << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "合约在交易所的代码, "
      << "ExchangeInstID, " << data.ExchangeInstID << endl
      << "最新价, "
      << "LastPrice, " << data.LastPrice << endl
      << "上次结算价, "
      << "PreSettlementPrice, " << data.PreSettlementPrice << endl
      << "昨收盘, "
      << "PreClosePrice, " << data.PreClosePrice << endl
      << "昨持仓量, "
      << "PreOpenInterest, " << data.PreOpenInterest << endl
      << "今开盘, "
      << "OpenPrice, " << data.OpenPrice << endl
      << "最高价, "
      << "HighestPrice, " << data.HighestPrice << endl
      << "最低价, "
      << "LowestPrice, " << data.LowestPrice << endl
      << "数量, "
      << "Volume, " << data.Volume << endl
      << "成交金额, "
      << "Turnover, " << data.Turnover << endl
      << "持仓量, "
      << "OpenInterest, " << data.OpenInterest << endl
      << "今收盘, "
      << "ClosePrice, " << data.ClosePrice << endl
      << "本次结算价, "
      << "SettlementPrice, " << data.SettlementPrice << endl
      << "涨停板价, "
      << "UpperLimitPrice, " << data.UpperLimitPrice << endl
      << "跌停板价, "
      << "LowerLimitPrice, " << data.LowerLimitPrice << endl
      << "昨虚实度, "
      << "PreDelta, " << data.PreDelta << endl
      << "今虚实度, "
      << "CurrDelta, " << data.CurrDelta << endl
      << "最后修改时间, "
      << "UpdateTime, " << data.UpdateTime << endl
      << "最后修改毫秒, "
      << "UpdateMillisec, " << data.UpdateMillisec << endl;
  return out;
}

/* 行情基础属性 */
ostream &operator<<(ostream &out, CThostFtdcMarketDataBaseField &data) {
  out << "CThostFtdcMarketDataBaseField" << endl
      << "交易日, "
      << "TradingDay, " << data.TradingDay << endl
      << "上次结算价, "
      << "PreSettlementPrice, " << data.PreSettlementPrice << endl
      << "昨收盘, "
      << "PreClosePrice, " << data.PreClosePrice << endl
      << "昨持仓量, "
      << "PreOpenInterest, " << data.PreOpenInterest << endl
      << "昨虚实度, "
      << "PreDelta, " << data.PreDelta << endl;
  return out;
}

/* 行情静态属性 */
ostream &operator<<(ostream &out, CThostFtdcMarketDataStaticField &data) {
  out << "CThostFtdcMarketDataStaticField" << endl
      << "今开盘, "
      << "OpenPrice, " << data.OpenPrice << endl
      << "最高价, "
      << "HighestPrice, " << data.HighestPrice << endl
      << "最低价, "
      << "LowestPrice, " << data.LowestPrice << endl
      << "今收盘, "
      << "ClosePrice, " << data.ClosePrice << endl
      << "涨停板价, "
      << "UpperLimitPrice, " << data.UpperLimitPrice << endl
      << "跌停板价, "
      << "LowerLimitPrice, " << data.LowerLimitPrice << endl
      << "本次结算价, "
      << "SettlementPrice, " << data.SettlementPrice << endl
      << "今虚实度, "
      << "CurrDelta, " << data.CurrDelta << endl;
  return out;
}

/* 行情最新成交属性 */
ostream &operator<<(ostream &out, CThostFtdcMarketDataLastMatchField &data) {
  out << "CThostFtdcMarketDataLastMatchField" << endl
      << "最新价, "
      << "LastPrice, " << data.LastPrice << endl
      << "数量, "
      << "Volume, " << data.Volume << endl
      << "成交金额, "
      << "Turnover, " << data.Turnover << endl
      << "持仓量, "
      << "OpenInterest, " << data.OpenInterest << endl;
  return out;
}

/* 行情最优价属性 */
ostream &operator<<(ostream &out, CThostFtdcMarketDataBestPriceField &data) {
  out << "CThostFtdcMarketDataBestPriceField" << endl
      << "申买价一, "
      << "BidPrice1, " << data.BidPrice1 << endl
      << "申买量一, "
      << "BidVolume1, " << data.BidVolume1 << endl
      << "申卖价一, "
      << "AskPrice1, " << data.AskPrice1 << endl
      << "申卖量一, "
      << "AskVolume1, " << data.AskVolume1 << endl;
  return out;
}

/* 行情申买二、三属性 */
ostream &operator<<(ostream &out, CThostFtdcMarketDataBid23Field &data) {
  out << "CThostFtdcMarketDataBid23Field" << endl
      << "申买价二, "
      << "BidPrice2, " << data.BidPrice2 << endl
      << "申买量二, "
      << "BidVolume2, " << data.BidVolume2 << endl
      << "申买价三, "
      << "BidPrice3, " << data.BidPrice3 << endl
      << "申买量三, "
      << "BidVolume3, " << data.BidVolume3 << endl;
  return out;
}

/* 行情申卖二、三属性 */
ostream &operator<<(ostream &out, CThostFtdcMarketDataAsk23Field &data) {
  out << "CThostFtdcMarketDataAsk23Field" << endl
      << "申卖价二, "
      << "AskPrice2, " << data.AskPrice2 << endl
      << "申卖量二, "
      << "AskVolume2, " << data.AskVolume2 << endl
      << "申卖价三, "
      << "AskPrice3, " << data.AskPrice3 << endl
      << "申卖量三, "
      << "AskVolume3, " << data.AskVolume3 << endl;
  return out;
}

/* 行情申买四、五属性 */
ostream &operator<<(ostream &out, CThostFtdcMarketDataBid45Field &data) {
  out << "CThostFtdcMarketDataBid45Field" << endl
      << "申买价四, "
      << "BidPrice4, " << data.BidPrice4 << endl
      << "申买量四, "
      << "BidVolume4, " << data.BidVolume4 << endl
      << "申买价五, "
      << "BidPrice5, " << data.BidPrice5 << endl
      << "申买量五, "
      << "BidVolume5, " << data.BidVolume5 << endl;
  return out;
}

/* 行情申卖四、五属性 */
ostream &operator<<(ostream &out, CThostFtdcMarketDataAsk45Field &data) {
  out << "CThostFtdcMarketDataAsk45Field" << endl
      << "申卖价四, "
      << "AskPrice4, " << data.AskPrice4 << endl
      << "申卖量四, "
      << "AskVolume4, " << data.AskVolume4 << endl
      << "申卖价五, "
      << "AskPrice5, " << data.AskPrice5 << endl
      << "申卖量五, "
      << "AskVolume5, " << data.AskVolume5 << endl;
  return out;
}

/* 行情更新时间属性 */
ostream &operator<<(ostream &out, CThostFtdcMarketDataUpdateTimeField &data) {
  out << "CThostFtdcMarketDataUpdateTimeField" << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "最后修改时间, "
      << "UpdateTime, " << data.UpdateTime << endl
      << "最后修改毫秒, "
      << "UpdateMillisec, " << data.UpdateMillisec << endl;
  return out;
}

/* 指定的合约 */
ostream &operator<<(ostream &out, CThostFtdcSpecificInstrumentField &data) {
  out << "CThostFtdcSpecificInstrumentField" << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl;
  return out;
}

/* 合约状态 */
ostream &operator<<(ostream &out, CThostFtdcInstrumentStatusField &data) {
  out << "CThostFtdcInstrumentStatusField" << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "合约在交易所的代码, "
      << "ExchangeInstID, " << data.ExchangeInstID << endl
      << "结算组代码, "
      << "SettlementGroupID, " << data.SettlementGroupID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "合约交易状态, "
      << "InstrumentStatus, " << data.InstrumentStatus << endl
      << "交易阶段编号, "
      << "TradingSegmentSN, " << data.TradingSegmentSN << endl
      << "进入本状态时间, "
      << "EnterTime, " << data.EnterTime << endl
      << "进入本状态原因, "
      << "EnterReason, " << data.EnterReason << endl;
  return out;
}

/* 查询合约状态 */
ostream &operator<<(ostream &out, CThostFtdcQryInstrumentStatusField &data) {
  out << "CThostFtdcQryInstrumentStatusField" << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "合约在交易所的代码, "
      << "ExchangeInstID, " << data.ExchangeInstID << endl;
  return out;
}

/* 投资者账户 */
ostream &operator<<(ostream &out, CThostFtdcInvestorAccountField &data) {
  out << "CThostFtdcInvestorAccountField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "投资者帐号, "
      << "AccountID, " << data.AccountID << endl;
  return out;
}

/* 浮动盈亏算法 */
ostream &operator<<(ostream &out,
                    CThostFtdcPositionProfitAlgorithmField &data) {
  out << "CThostFtdcPositionProfitAlgorithmField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者帐号, "
      << "AccountID, " << data.AccountID << endl
      << "盈亏算法, "
      << "Algorithm, " << data.Algorithm << endl
      << "备注, "
      << "Memo, " << data.Memo << endl;
  return out;
}

/* 会员资金折扣 */
ostream &operator<<(ostream &out, CThostFtdcDiscountField &data) {
  out << "CThostFtdcDiscountField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者范围, "
      << "InvestorRange, " << data.InvestorRange << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "资金折扣比例, "
      << "Discount, " << data.Discount << endl;
  return out;
}

/* 查询转帐银行 */
ostream &operator<<(ostream &out, CThostFtdcQryTransferBankField &data) {
  out << "CThostFtdcQryTransferBankField" << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分中心代码, "
      << "BankBrchID, " << data.BankBrchID << endl;
  return out;
}

/* 转帐银行 */
ostream &operator<<(ostream &out, CThostFtdcTransferBankField &data) {
  out << "CThostFtdcTransferBankField" << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分中心代码, "
      << "BankBrchID, " << data.BankBrchID << endl
      << "银行名称, "
      << "BankName, " << data.BankName << endl
      << "是否活跃, "
      << "IsActive, " << data.IsActive << endl;
  return out;
}

/* 查询投资者持仓明细 */
ostream &operator<<(ostream &out,
                    CThostFtdcQryInvestorPositionDetailField &data) {
  out << "CThostFtdcQryInvestorPositionDetailField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl;
  return out;
}

/* 投资者持仓明细 */
ostream &operator<<(ostream &out, CThostFtdcInvestorPositionDetailField &data) {
  out << "CThostFtdcInvestorPositionDetailField" << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "投机套保标志, "
      << "HedgeFlag, " << data.HedgeFlag << endl
      << "买卖, "
      << "Direction, " << data.Direction << endl
      << "开仓日期, "
      << "OpenDate, " << data.OpenDate << endl
      << "成交编号, "
      << "TradeID, " << data.TradeID << endl
      << "数量, "
      << "Volume, " << data.Volume << endl
      << "开仓价, "
      << "OpenPrice, " << data.OpenPrice << endl
      << "交易日, "
      << "TradingDay, " << data.TradingDay << endl
      << "结算编号, "
      << "SettlementID, " << data.SettlementID << endl
      << "成交类型, "
      << "TradeType, " << data.TradeType << endl
      << "组合合约代码, "
      << "CombInstrumentID, " << data.CombInstrumentID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "逐日盯市平仓盈亏, "
      << "CloseProfitByDate, " << data.CloseProfitByDate << endl
      << "逐笔对冲平仓盈亏, "
      << "CloseProfitByTrade, " << data.CloseProfitByTrade << endl
      << "逐日盯市持仓盈亏, "
      << "PositionProfitByDate, " << data.PositionProfitByDate << endl
      << "逐笔对冲持仓盈亏, "
      << "PositionProfitByTrade, " << data.PositionProfitByTrade << endl
      << "投资者保证金, "
      << "Margin, " << data.Margin << endl
      << "交易所保证金, "
      << "ExchMargin, " << data.ExchMargin << endl
      << "保证金率, "
      << "MarginRateByMoney, " << data.MarginRateByMoney << endl
      << "保证金率(按手数), "
      << "MarginRateByVolume, " << data.MarginRateByVolume << endl
      << "昨结算价, "
      << "LastSettlementPrice, " << data.LastSettlementPrice << endl
      << "结算价, "
      << "SettlementPrice, " << data.SettlementPrice << endl
      << "平仓量, "
      << "CloseVolume, " << data.CloseVolume << endl
      << "平仓金额, "
      << "CloseAmount, " << data.CloseAmount << endl;
  return out;
}

/* 资金账户口令域 */
ostream &operator<<(ostream &out, CThostFtdcTradingAccountPasswordField &data) {
  out << "CThostFtdcTradingAccountPasswordField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者帐号, "
      << "AccountID, " << data.AccountID << endl
      << "密码, "
      << "Password, " << data.Password << endl;
  return out;
}

/* 交易所行情报盘机 */
ostream &operator<<(ostream &out, CThostFtdcMDTraderOfferField &data) {
  out << "CThostFtdcMDTraderOfferField" << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "交易所交易员代码, "
      << "TraderID, " << data.TraderID << endl
      << "会员代码, "
      << "ParticipantID, " << data.ParticipantID << endl
      << "密码, "
      << "Password, " << data.Password << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "本地报单编号, "
      << "OrderLocalID, " << data.OrderLocalID << endl
      << "交易所交易员连接状态, "
      << "TraderConnectStatus, " << data.TraderConnectStatus << endl
      << "发出连接请求的日期, "
      << "ConnectRequestDate, " << data.ConnectRequestDate << endl
      << "发出连接请求的时间, "
      << "ConnectRequestTime, " << data.ConnectRequestTime << endl
      << "上次报告日期, "
      << "LastReportDate, " << data.LastReportDate << endl
      << "上次报告时间, "
      << "LastReportTime, " << data.LastReportTime << endl
      << "完成连接日期, "
      << "ConnectDate, " << data.ConnectDate << endl
      << "完成连接时间, "
      << "ConnectTime, " << data.ConnectTime << endl
      << "启动日期, "
      << "StartDate, " << data.StartDate << endl
      << "启动时间, "
      << "StartTime, " << data.StartTime << endl
      << "交易日, "
      << "TradingDay, " << data.TradingDay << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl;
  return out;
}

/* 查询行情报盘机 */
ostream &operator<<(ostream &out, CThostFtdcQryMDTraderOfferField &data) {
  out << "CThostFtdcQryMDTraderOfferField" << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "会员代码, "
      << "ParticipantID, " << data.ParticipantID << endl
      << "交易所交易员代码, "
      << "TraderID, " << data.TraderID << endl;
  return out;
}

/* 查询客户通知 */
ostream &operator<<(ostream &out, CThostFtdcQryNoticeField &data) {
  out << "CThostFtdcQryNoticeField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl;
  return out;
}

/* 客户通知 */
ostream &operator<<(ostream &out, CThostFtdcNoticeField &data) {
  out << "CThostFtdcNoticeField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "消息正文, "
      << "Content, " << data.Content << endl
      << "经纪公司通知内容序列号, "
      << "SequenceLabel, " << data.SequenceLabel << endl;
  return out;
}

/* 用户权限 */
ostream &operator<<(ostream &out, CThostFtdcUserRightField &data) {
  out << "CThostFtdcUserRightField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "客户权限类型, "
      << "UserRightType, " << data.UserRightType << endl
      << "是否禁止, "
      << "IsForbidden, " << data.IsForbidden << endl;
  return out;
}

/* 查询结算信息确认域 */
ostream &operator<<(ostream &out,
                    CThostFtdcQrySettlementInfoConfirmField &data) {
  out << "CThostFtdcQrySettlementInfoConfirmField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl;
  return out;
}

/* 装载结算信息 */
ostream &operator<<(ostream &out, CThostFtdcLoadSettlementInfoField &data) {
  out << "CThostFtdcLoadSettlementInfoField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl;
  return out;
}

/* 经纪公司可提资金算法表 */
ostream &operator<<(ostream &out,
                    CThostFtdcBrokerWithdrawAlgorithmField &data) {
  out << "CThostFtdcBrokerWithdrawAlgorithmField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "可提资金算法, "
      << "WithdrawAlgorithm, " << data.WithdrawAlgorithm << endl
      << "资金使用率, "
      << "UsingRatio, " << data.UsingRatio << endl
      << "可提是否包含平仓盈利, "
      << "IncludeCloseProfit, " << data.IncludeCloseProfit << endl
      << "本日无仓且无成交客户是否受可提比例限制, "
      << "AllWithoutTrade, " << data.AllWithoutTrade << endl
      << "可用是否包含平仓盈利, "
      << "AvailIncludeCloseProfit, " << data.AvailIncludeCloseProfit << endl
      << "是否启用用户事件, "
      << "IsBrokerUserEvent, " << data.IsBrokerUserEvent << endl;
  return out;
}

/* 资金账户口令变更域 */
ostream &operator<<(ostream &out,
                    CThostFtdcTradingAccountPasswordUpdateV1Field &data) {
  out << "CThostFtdcTradingAccountPasswordUpdateV1Field" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "原来的口令, "
      << "OldPassword, " << data.OldPassword << endl
      << "新的口令, "
      << "NewPassword, " << data.NewPassword << endl;
  return out;
}

/* 资金账户口令变更域 */
ostream &operator<<(ostream &out,
                    CThostFtdcTradingAccountPasswordUpdateField &data) {
  out << "CThostFtdcTradingAccountPasswordUpdateField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者帐号, "
      << "AccountID, " << data.AccountID << endl
      << "原来的口令, "
      << "OldPassword, " << data.OldPassword << endl
      << "新的口令, "
      << "NewPassword, " << data.NewPassword << endl;
  return out;
}

/* 查询组合合约分腿 */
ostream &operator<<(ostream &out, CThostFtdcQryCombinationLegField &data) {
  out << "CThostFtdcQryCombinationLegField" << endl
      << "组合合约代码, "
      << "CombInstrumentID, " << data.CombInstrumentID << endl
      << "单腿编号, "
      << "LegID, " << data.LegID << endl
      << "单腿合约代码, "
      << "LegInstrumentID, " << data.LegInstrumentID << endl;
  return out;
}

/* 查询组合合约分腿 */
ostream &operator<<(ostream &out, CThostFtdcQrySyncStatusField &data) {
  out << "CThostFtdcQrySyncStatusField" << endl
      << "交易日, "
      << "TradingDay, " << data.TradingDay << endl;
  return out;
}

/* 组合交易合约的单腿 */
ostream &operator<<(ostream &out, CThostFtdcCombinationLegField &data) {
  out << "CThostFtdcCombinationLegField" << endl
      << "组合合约代码, "
      << "CombInstrumentID, " << data.CombInstrumentID << endl
      << "单腿编号, "
      << "LegID, " << data.LegID << endl
      << "单腿合约代码, "
      << "LegInstrumentID, " << data.LegInstrumentID << endl
      << "买卖方向, "
      << "Direction, " << data.Direction << endl
      << "单腿乘数, "
      << "LegMultiple, " << data.LegMultiple << endl
      << "派生层数, "
      << "ImplyLevel, " << data.ImplyLevel << endl;
  return out;
}

/* 数据同步状态 */
ostream &operator<<(ostream &out, CThostFtdcSyncStatusField &data) {
  out << "CThostFtdcSyncStatusField" << endl
      << "交易日, "
      << "TradingDay, " << data.TradingDay << endl
      << "数据同步状态, "
      << "DataSyncStatus, " << data.DataSyncStatus << endl;
  return out;
}

/* 查询联系人 */
ostream &operator<<(ostream &out, CThostFtdcQryLinkManField &data) {
  out << "CThostFtdcQryLinkManField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl;
  return out;
}

/* 联系人 */
ostream &operator<<(ostream &out, CThostFtdcLinkManField &data) {
  out << "CThostFtdcLinkManField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "联系人类型, "
      << "PersonType, " << data.PersonType << endl
      << "证件类型, "
      << "IdentifiedCardType, " << data.IdentifiedCardType << endl
      << "证件号码, "
      << "IdentifiedCardNo, " << data.IdentifiedCardNo << endl
      << "名称, "
      << "PersonName, " << data.PersonName << endl
      << "联系电话, "
      << "Telephone, " << data.Telephone << endl
      << "通讯地址, "
      << "Address, " << data.Address << endl
      << "邮政编码, "
      << "ZipCode, " << data.ZipCode << endl
      << "优先级, "
      << "Priority, " << data.Priority << endl;
  return out;
}

/* 查询经纪公司用户事件 */
ostream &operator<<(ostream &out, CThostFtdcQryBrokerUserEventField &data) {
  out << "CThostFtdcQryBrokerUserEventField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "用户事件类型, "
      << "UserEventType, " << data.UserEventType << endl;
  return out;
}

/* 查询经纪公司用户事件 */
ostream &operator<<(ostream &out, CThostFtdcBrokerUserEventField &data) {
  out << "CThostFtdcBrokerUserEventField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "用户事件类型, "
      << "UserEventType, " << data.UserEventType << endl
      << "用户事件序号, "
      << "EventSequenceNo, " << data.EventSequenceNo << endl
      << "事件发生日期, "
      << "EventDate, " << data.EventDate << endl
      << "事件发生时间, "
      << "EventTime, " << data.EventTime << endl
      << "用户事件信息, "
      << "UserEventInfo, " << data.UserEventInfo << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl;
  return out;
}

/* 查询签约银行请求 */
ostream &operator<<(ostream &out, CThostFtdcQryContractBankField &data) {
  out << "CThostFtdcQryContractBankField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分中心代码, "
      << "BankBrchID, " << data.BankBrchID << endl;
  return out;
}

/* 查询签约银行响应 */
ostream &operator<<(ostream &out, CThostFtdcContractBankField &data) {
  out << "CThostFtdcContractBankField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分中心代码, "
      << "BankBrchID, " << data.BankBrchID << endl
      << "银行名称, "
      << "BankName, " << data.BankName << endl;
  return out;
}

/* 投资者组合持仓明细 */
ostream &operator<<(ostream &out,
                    CThostFtdcInvestorPositionCombineDetailField &data) {
  out << "CThostFtdcInvestorPositionCombineDetailField" << endl
      << "交易日, "
      << "TradingDay, " << data.TradingDay << endl
      << "开仓日期, "
      << "OpenDate, " << data.OpenDate << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "结算编号, "
      << "SettlementID, " << data.SettlementID << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "组合编号, "
      << "ComTradeID, " << data.ComTradeID << endl
      << "撮合编号, "
      << "TradeID, " << data.TradeID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "投机套保标志, "
      << "HedgeFlag, " << data.HedgeFlag << endl
      << "买卖, "
      << "Direction, " << data.Direction << endl
      << "持仓量, "
      << "TotalAmt, " << data.TotalAmt << endl
      << "投资者保证金, "
      << "Margin, " << data.Margin << endl
      << "交易所保证金, "
      << "ExchMargin, " << data.ExchMargin << endl
      << "保证金率, "
      << "MarginRateByMoney, " << data.MarginRateByMoney << endl
      << "保证金率(按手数), "
      << "MarginRateByVolume, " << data.MarginRateByVolume << endl
      << "单腿编号, "
      << "LegID, " << data.LegID << endl
      << "单腿乘数, "
      << "LegMultiple, " << data.LegMultiple << endl
      << "组合持仓合约编码, "
      << "CombInstrumentID, " << data.CombInstrumentID << endl;
  return out;
}

/* 预埋单 */
ostream &operator<<(ostream &out, CThostFtdcParkedOrderField &data) {
  out << "CThostFtdcParkedOrderField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "报单引用, "
      << "OrderRef, " << data.OrderRef << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "报单价格条件, "
      << "OrderPriceType, " << data.OrderPriceType << endl
      << "买卖方向, "
      << "Direction, " << data.Direction << endl
      << "组合开平标志, "
      << "CombOffsetFlag, " << data.CombOffsetFlag << endl
      << "组合投机套保标志, "
      << "CombHedgeFlag, " << data.CombHedgeFlag << endl
      << "价格, "
      << "LimitPrice, " << data.LimitPrice << endl
      << "数量, "
      << "VolumeTotalOriginal, " << data.VolumeTotalOriginal << endl
      << "有效期类型, "
      << "TimeCondition, " << data.TimeCondition << endl
      << "GTD日期, "
      << "GTDDate, " << data.GTDDate << endl
      << "成交量类型, "
      << "VolumeCondition, " << data.VolumeCondition << endl
      << "最小成交量, "
      << "MinVolume, " << data.MinVolume << endl
      << "触发条件, "
      << "ContingentCondition, " << data.ContingentCondition << endl
      << "止损价, "
      << "StopPrice, " << data.StopPrice << endl
      << "强平原因, "
      << "ForceCloseReason, " << data.ForceCloseReason << endl
      << "自动挂起标志, "
      << "IsAutoSuspend, " << data.IsAutoSuspend << endl
      << "业务单元, "
      << "BusinessUnit, " << data.BusinessUnit << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "用户强评标志, "
      << "UserForceClose, " << data.UserForceClose << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "预埋报单编号, "
      << "ParkedOrderID, " << data.ParkedOrderID << endl
      << "用户类型, "
      << "UserType, " << data.UserType << endl
      << "预埋单状态, "
      << "Status, " << data.Status << endl
      << "错误代码, "
      << "ErrorID, " << data.ErrorID << endl
      << "错误信息, "
      << "ErrorMsg, " << data.ErrorMsg << endl;
  return out;
}

/* 输入预埋单操作 */
ostream &operator<<(ostream &out, CThostFtdcParkedOrderActionField &data) {
  out << "CThostFtdcParkedOrderActionField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "报单操作引用, "
      << "OrderActionRef, " << data.OrderActionRef << endl
      << "报单引用, "
      << "OrderRef, " << data.OrderRef << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "前置编号, "
      << "FrontID, " << data.FrontID << endl
      << "会话编号, "
      << "SessionID, " << data.SessionID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "报单编号, "
      << "OrderSysID, " << data.OrderSysID << endl
      << "操作标志, "
      << "ActionFlag, " << data.ActionFlag << endl
      << "价格, "
      << "LimitPrice, " << data.LimitPrice << endl
      << "数量变化, "
      << "VolumeChange, " << data.VolumeChange << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "预埋撤单单编号, "
      << "ParkedOrderActionID, " << data.ParkedOrderActionID << endl
      << "用户类型, "
      << "UserType, " << data.UserType << endl
      << "预埋撤单状态, "
      << "Status, " << data.Status << endl
      << "错误代码, "
      << "ErrorID, " << data.ErrorID << endl
      << "错误信息, "
      << "ErrorMsg, " << data.ErrorMsg << endl;
  return out;
}

/* 查询预埋单 */
ostream &operator<<(ostream &out, CThostFtdcQryParkedOrderField &data) {
  out << "CThostFtdcQryParkedOrderField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl;
  return out;
}

/* 查询预埋撤单 */
ostream &operator<<(ostream &out, CThostFtdcQryParkedOrderActionField &data) {
  out << "CThostFtdcQryParkedOrderActionField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl;
  return out;
}

/* 删除预埋单 */
ostream &operator<<(ostream &out, CThostFtdcRemoveParkedOrderField &data) {
  out << "CThostFtdcRemoveParkedOrderField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "预埋报单编号, "
      << "ParkedOrderID, " << data.ParkedOrderID << endl;
  return out;
}

/* 删除预埋撤单 */
ostream &operator<<(ostream &out,
                    CThostFtdcRemoveParkedOrderActionField &data) {
  out << "CThostFtdcRemoveParkedOrderActionField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "预埋撤单编号, "
      << "ParkedOrderActionID, " << data.ParkedOrderActionID << endl;
  return out;
}

/* 经纪公司可提资金算法表 */
ostream &operator<<(ostream &out,
                    CThostFtdcInvestorWithdrawAlgorithmField &data) {
  out << "CThostFtdcInvestorWithdrawAlgorithmField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者范围, "
      << "InvestorRange, " << data.InvestorRange << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "可提资金比例, "
      << "UsingRatio, " << data.UsingRatio << endl;
  return out;
}

/* 查询组合持仓明细 */
ostream &operator<<(ostream &out,
                    CThostFtdcQryInvestorPositionCombineDetailField &data) {
  out << "CThostFtdcQryInvestorPositionCombineDetailField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "组合持仓合约编码, "
      << "CombInstrumentID, " << data.CombInstrumentID << endl;
  return out;
}

/* 成交均价 */
ostream &operator<<(ostream &out, CThostFtdcMarketDataAveragePriceField &data) {
  out << "CThostFtdcMarketDataAveragePriceField" << endl
      << "当日均价, "
      << "AveragePrice, " << data.AveragePrice << endl;
  return out;
}

/* 校验投资者密码 */
ostream &operator<<(ostream &out, CThostFtdcVerifyInvestorPasswordField &data) {
  out << "CThostFtdcVerifyInvestorPasswordField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "密码, "
      << "Password, " << data.Password << endl;
  return out;
}

/* 用户IP */
ostream &operator<<(ostream &out, CThostFtdcUserIPField &data) {
  out << "CThostFtdcUserIPField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "IP地址, "
      << "IPAddress, " << data.IPAddress << endl
      << "IP地址掩码, "
      << "IPMask, " << data.IPMask << endl
      << "Mac地址, "
      << "MacAddress, " << data.MacAddress << endl;
  return out;
}

/* 用户事件通知信息 */
ostream &operator<<(ostream &out, CThostFtdcTradingNoticeInfoField &data) {
  out << "CThostFtdcTradingNoticeInfoField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "发送时间, "
      << "SendTime, " << data.SendTime << endl
      << "消息正文, "
      << "FieldContent, " << data.FieldContent << endl
      << "序列系列号, "
      << "SequenceSeries, " << data.SequenceSeries << endl
      << "序列号, "
      << "SequenceNo, " << data.SequenceNo << endl;
  return out;
}

/* 用户事件通知 */
ostream &operator<<(ostream &out, CThostFtdcTradingNoticeField &data) {
  out << "CThostFtdcTradingNoticeField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者范围, "
      << "InvestorRange, " << data.InvestorRange << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "序列系列号, "
      << "SequenceSeries, " << data.SequenceSeries << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "发送时间, "
      << "SendTime, " << data.SendTime << endl
      << "序列号, "
      << "SequenceNo, " << data.SequenceNo << endl
      << "消息正文, "
      << "FieldContent, " << data.FieldContent << endl;
  return out;
}

/* 查询交易事件通知 */
ostream &operator<<(ostream &out, CThostFtdcQryTradingNoticeField &data) {
  out << "CThostFtdcQryTradingNoticeField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl;
  return out;
}

/* 查询错误报单 */
ostream &operator<<(ostream &out, CThostFtdcQryErrOrderField &data) {
  out << "CThostFtdcQryErrOrderField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl;
  return out;
}

/* 错误报单 */
ostream &operator<<(ostream &out, CThostFtdcErrOrderField &data) {
  out << "CThostFtdcErrOrderField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "报单引用, "
      << "OrderRef, " << data.OrderRef << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "报单价格条件, "
      << "OrderPriceType, " << data.OrderPriceType << endl
      << "买卖方向, "
      << "Direction, " << data.Direction << endl
      << "组合开平标志, "
      << "CombOffsetFlag, " << data.CombOffsetFlag << endl
      << "组合投机套保标志, "
      << "CombHedgeFlag, " << data.CombHedgeFlag << endl
      << "价格, "
      << "LimitPrice, " << data.LimitPrice << endl
      << "数量, "
      << "VolumeTotalOriginal, " << data.VolumeTotalOriginal << endl
      << "有效期类型, "
      << "TimeCondition, " << data.TimeCondition << endl
      << "GTD日期, "
      << "GTDDate, " << data.GTDDate << endl
      << "成交量类型, "
      << "VolumeCondition, " << data.VolumeCondition << endl
      << "最小成交量, "
      << "MinVolume, " << data.MinVolume << endl
      << "触发条件, "
      << "ContingentCondition, " << data.ContingentCondition << endl
      << "止损价, "
      << "StopPrice, " << data.StopPrice << endl
      << "强平原因, "
      << "ForceCloseReason, " << data.ForceCloseReason << endl
      << "自动挂起标志, "
      << "IsAutoSuspend, " << data.IsAutoSuspend << endl
      << "业务单元, "
      << "BusinessUnit, " << data.BusinessUnit << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "用户强评标志, "
      << "UserForceClose, " << data.UserForceClose << endl
      << "错误代码, "
      << "ErrorID, " << data.ErrorID << endl
      << "错误信息, "
      << "ErrorMsg, " << data.ErrorMsg << endl;
  return out;
}

/* 查询错误报单操作 */
ostream &operator<<(ostream &out, CThostFtdcErrorConditionalOrderField &data) {
  out << "CThostFtdcErrorConditionalOrderField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "报单引用, "
      << "OrderRef, " << data.OrderRef << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "报单价格条件, "
      << "OrderPriceType, " << data.OrderPriceType << endl
      << "买卖方向, "
      << "Direction, " << data.Direction << endl
      << "组合开平标志, "
      << "CombOffsetFlag, " << data.CombOffsetFlag << endl
      << "组合投机套保标志, "
      << "CombHedgeFlag, " << data.CombHedgeFlag << endl
      << "价格, "
      << "LimitPrice, " << data.LimitPrice << endl
      << "数量, "
      << "VolumeTotalOriginal, " << data.VolumeTotalOriginal << endl
      << "有效期类型, "
      << "TimeCondition, " << data.TimeCondition << endl
      << "GTD日期, "
      << "GTDDate, " << data.GTDDate << endl
      << "成交量类型, "
      << "VolumeCondition, " << data.VolumeCondition << endl
      << "最小成交量, "
      << "MinVolume, " << data.MinVolume << endl
      << "触发条件, "
      << "ContingentCondition, " << data.ContingentCondition << endl
      << "止损价, "
      << "StopPrice, " << data.StopPrice << endl
      << "强平原因, "
      << "ForceCloseReason, " << data.ForceCloseReason << endl
      << "自动挂起标志, "
      << "IsAutoSuspend, " << data.IsAutoSuspend << endl
      << "业务单元, "
      << "BusinessUnit, " << data.BusinessUnit << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "本地报单编号, "
      << "OrderLocalID, " << data.OrderLocalID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "会员代码, "
      << "ParticipantID, " << data.ParticipantID << endl
      << "客户代码, "
      << "ClientID, " << data.ClientID << endl
      << "合约在交易所的代码, "
      << "ExchangeInstID, " << data.ExchangeInstID << endl
      << "交易所交易员代码, "
      << "TraderID, " << data.TraderID << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "报单提交状态, "
      << "OrderSubmitStatus, " << data.OrderSubmitStatus << endl
      << "报单提示序号, "
      << "NotifySequence, " << data.NotifySequence << endl
      << "交易日, "
      << "TradingDay, " << data.TradingDay << endl
      << "结算编号, "
      << "SettlementID, " << data.SettlementID << endl
      << "报单编号, "
      << "OrderSysID, " << data.OrderSysID << endl
      << "报单来源, "
      << "OrderSource, " << data.OrderSource << endl
      << "报单状态, "
      << "OrderStatus, " << data.OrderStatus << endl
      << "报单类型, "
      << "OrderType, " << data.OrderType << endl
      << "今成交数量, "
      << "VolumeTraded, " << data.VolumeTraded << endl
      << "剩余数量, "
      << "VolumeTotal, " << data.VolumeTotal << endl
      << "报单日期, "
      << "InsertDate, " << data.InsertDate << endl
      << "委托时间, "
      << "InsertTime, " << data.InsertTime << endl
      << "激活时间, "
      << "ActiveTime, " << data.ActiveTime << endl
      << "挂起时间, "
      << "SuspendTime, " << data.SuspendTime << endl
      << "最后修改时间, "
      << "UpdateTime, " << data.UpdateTime << endl
      << "撤销时间, "
      << "CancelTime, " << data.CancelTime << endl
      << "最后修改交易所交易员代码, "
      << "ActiveTraderID, " << data.ActiveTraderID << endl
      << "结算会员编号, "
      << "ClearingPartID, " << data.ClearingPartID << endl
      << "序号, "
      << "SequenceNo, " << data.SequenceNo << endl
      << "前置编号, "
      << "FrontID, " << data.FrontID << endl
      << "会话编号, "
      << "SessionID, " << data.SessionID << endl
      << "用户端产品信息, "
      << "UserProductInfo, " << data.UserProductInfo << endl
      << "状态信息, "
      << "StatusMsg, " << data.StatusMsg << endl
      << "用户强评标志, "
      << "UserForceClose, " << data.UserForceClose << endl
      << "操作用户代码, "
      << "ActiveUserID, " << data.ActiveUserID << endl
      << "经纪公司报单编号, "
      << "BrokerOrderSeq, " << data.BrokerOrderSeq << endl
      << "相关报单, "
      << "RelativeOrderSysID, " << data.RelativeOrderSysID << endl
      << "错误代码, "
      << "ErrorID, " << data.ErrorID << endl
      << "错误信息, "
      << "ErrorMsg, " << data.ErrorMsg << endl;
  return out;
}

/* 查询错误报单操作 */
ostream &operator<<(ostream &out, CThostFtdcQryErrOrderActionField &data) {
  out << "CThostFtdcQryErrOrderActionField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl;
  return out;
}

/* 错误报单操作 */
ostream &operator<<(ostream &out, CThostFtdcErrOrderActionField &data) {
  out << "CThostFtdcErrOrderActionField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "报单操作引用, "
      << "OrderActionRef, " << data.OrderActionRef << endl
      << "报单引用, "
      << "OrderRef, " << data.OrderRef << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "前置编号, "
      << "FrontID, " << data.FrontID << endl
      << "会话编号, "
      << "SessionID, " << data.SessionID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "报单编号, "
      << "OrderSysID, " << data.OrderSysID << endl
      << "操作标志, "
      << "ActionFlag, " << data.ActionFlag << endl
      << "价格, "
      << "LimitPrice, " << data.LimitPrice << endl
      << "数量变化, "
      << "VolumeChange, " << data.VolumeChange << endl
      << "操作日期, "
      << "ActionDate, " << data.ActionDate << endl
      << "操作时间, "
      << "ActionTime, " << data.ActionTime << endl
      << "交易所交易员代码, "
      << "TraderID, " << data.TraderID << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "本地报单编号, "
      << "OrderLocalID, " << data.OrderLocalID << endl
      << "操作本地编号, "
      << "ActionLocalID, " << data.ActionLocalID << endl
      << "会员代码, "
      << "ParticipantID, " << data.ParticipantID << endl
      << "客户代码, "
      << "ClientID, " << data.ClientID << endl
      << "业务单元, "
      << "BusinessUnit, " << data.BusinessUnit << endl
      << "报单操作状态, "
      << "OrderActionStatus, " << data.OrderActionStatus << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "状态信息, "
      << "StatusMsg, " << data.StatusMsg << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "错误代码, "
      << "ErrorID, " << data.ErrorID << endl
      << "错误信息, "
      << "ErrorMsg, " << data.ErrorMsg << endl;
  return out;
}

/* 查询交易所状态 */
ostream &operator<<(ostream &out, CThostFtdcQryExchangeSequenceField &data) {
  out << "CThostFtdcQryExchangeSequenceField" << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl;
  return out;
}

/* 交易所状态 */
ostream &operator<<(ostream &out, CThostFtdcExchangeSequenceField &data) {
  out << "CThostFtdcExchangeSequenceField" << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "序号, "
      << "SequenceNo, " << data.SequenceNo << endl
      << "合约交易状态, "
      << "MarketStatus, " << data.MarketStatus << endl;
  return out;
}

/* 根据价格查询最大报单数量 */
ostream &operator<<(ostream &out,
                    CThostFtdcQueryMaxOrderVolumeWithPriceField &data) {
  out << "CThostFtdcQueryMaxOrderVolumeWithPriceField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "买卖方向, "
      << "Direction, " << data.Direction << endl
      << "开平标志, "
      << "OffsetFlag, " << data.OffsetFlag << endl
      << "投机套保标志, "
      << "HedgeFlag, " << data.HedgeFlag << endl
      << "最大允许报单数量, "
      << "MaxVolume, " << data.MaxVolume << endl
      << "报单价格, "
      << "Price, " << data.Price << endl;
  return out;
}

/* 查询经纪公司交易参数 */
ostream &operator<<(ostream &out, CThostFtdcQryBrokerTradingParamsField &data) {
  out << "CThostFtdcQryBrokerTradingParamsField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl;
  return out;
}

/* 经纪公司交易参数 */
ostream &operator<<(ostream &out, CThostFtdcBrokerTradingParamsField &data) {
  out << "CThostFtdcBrokerTradingParamsField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "保证金价格类型, "
      << "MarginPriceType, " << data.MarginPriceType << endl
      << "盈亏算法, "
      << "Algorithm, " << data.Algorithm << endl
      << "可用是否包含平仓盈利, "
      << "AvailIncludeCloseProfit, " << data.AvailIncludeCloseProfit << endl;
  return out;
}

/* 查询经纪公司交易算法 */
ostream &operator<<(ostream &out, CThostFtdcQryBrokerTradingAlgosField &data) {
  out << "CThostFtdcQryBrokerTradingAlgosField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl;
  return out;
}

/* 经纪公司交易算法 */
ostream &operator<<(ostream &out, CThostFtdcBrokerTradingAlgosField &data) {
  out << "CThostFtdcBrokerTradingAlgosField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "合约代码, "
      << "InstrumentID, " << data.InstrumentID << endl
      << "持仓处理算法编号, "
      << "HandlePositionAlgoID, " << data.HandlePositionAlgoID << endl
      << "寻找保证金率算法编号, "
      << "FindMarginRateAlgoID, " << data.FindMarginRateAlgoID << endl
      << "资金处理算法编号, "
      << "HandleTradingAccountAlgoID, " << data.HandleTradingAccountAlgoID
      << endl;
  return out;
}

/* 查询经纪公司资金 */
ostream &operator<<(ostream &out, CThostFtdcQueryBrokerDepositField &data) {
  out << "CThostFtdcQueryBrokerDepositField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl;
  return out;
}

/* 经纪公司资金 */
ostream &operator<<(ostream &out, CThostFtdcBrokerDepositField &data) {
  out << "CThostFtdcBrokerDepositField" << endl
      << "交易日期, "
      << "TradingDay, " << data.TradingDay << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "会员代码, "
      << "ParticipantID, " << data.ParticipantID << endl
      << "交易所代码, "
      << "ExchangeID, " << data.ExchangeID << endl
      << "上次结算准备金, "
      << "PreBalance, " << data.PreBalance << endl
      << "当前保证金总额, "
      << "CurrMargin, " << data.CurrMargin << endl
      << "平仓盈亏, "
      << "CloseProfit, " << data.CloseProfit << endl
      << "期货结算准备金, "
      << "Balance, " << data.Balance << endl
      << "入金金额, "
      << "Deposit, " << data.Deposit << endl
      << "出金金额, "
      << "Withdraw, " << data.Withdraw << endl
      << "可提资金, "
      << "Available, " << data.Available << endl
      << "基本准备金, "
      << "Reserve, " << data.Reserve << endl
      << "冻结的保证金, "
      << "FrozenMargin, " << data.FrozenMargin << endl;
  return out;
}

/* 查询保证金监管系统经纪公司密钥 */
ostream &operator<<(ostream &out, CThostFtdcQryCFMMCBrokerKeyField &data) {
  out << "CThostFtdcQryCFMMCBrokerKeyField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl;
  return out;
}

/* 保证金监管系统经纪公司密钥 */
ostream &operator<<(ostream &out, CThostFtdcCFMMCBrokerKeyField &data) {
  out << "CThostFtdcCFMMCBrokerKeyField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "经纪公司统一编码, "
      << "ParticipantID, " << data.ParticipantID << endl
      << "密钥生成日期, "
      << "CreateDate, " << data.CreateDate << endl
      << "密钥生成时间, "
      << "CreateTime, " << data.CreateTime << endl
      << "密钥编号, "
      << "KeyID, " << data.KeyID << endl
      << "动态密钥, "
      << "CurrentKey, " << data.CurrentKey << endl
      << "动态密钥类型, "
      << "KeyKind, " << data.KeyKind << endl;
  return out;
}

/* 保证金监管系统经纪公司资金账户密钥 */
ostream &operator<<(ostream &out, CThostFtdcCFMMCTradingAccountKeyField &data) {
  out << "CThostFtdcCFMMCTradingAccountKeyField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "经纪公司统一编码, "
      << "ParticipantID, " << data.ParticipantID << endl
      << "投资者帐号, "
      << "AccountID, " << data.AccountID << endl
      << "密钥编号, "
      << "KeyID, " << data.KeyID << endl
      << "动态密钥, "
      << "CurrentKey, " << data.CurrentKey << endl;
  return out;
}

/* 请求查询保证金监管系统经纪公司资金账户密钥 */
ostream &operator<<(ostream &out,
                    CThostFtdcQryCFMMCTradingAccountKeyField &data) {
  out << "CThostFtdcQryCFMMCTradingAccountKeyField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl;
  return out;
}

/* 用户动态令牌参数 */
ostream &operator<<(ostream &out, CThostFtdcBrokerUserOTPParamField &data) {
  out << "CThostFtdcBrokerUserOTPParamField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "动态令牌提供商, "
      << "OTPVendorsID, " << data.OTPVendorsID << endl
      << "动态令牌序列号, "
      << "SerialNumber, " << data.SerialNumber << endl
      << "令牌密钥, "
      << "AuthKey, " << data.AuthKey << endl
      << "漂移值, "
      << "LastDrift, " << data.LastDrift << endl
      << "成功值, "
      << "LastSuccess, " << data.LastSuccess << endl
      << "动态令牌类型, "
      << "OTPType, " << data.OTPType << endl;
  return out;
}

/* 手工同步用户动态令牌 */
ostream &operator<<(ostream &out,
                    CThostFtdcManualSyncBrokerUserOTPField &data) {
  out << "CThostFtdcManualSyncBrokerUserOTPField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "用户代码, "
      << "UserID, " << data.UserID << endl
      << "动态令牌类型, "
      << "OTPType, " << data.OTPType << endl
      << "第一个动态密码, "
      << "FirstOTP, " << data.FirstOTP << endl
      << "第二个动态密码, "
      << "SecondOTP, " << data.SecondOTP << endl;
  return out;
}

/* 转帐开户请求 */
ostream &operator<<(ostream &out, CThostFtdcReqOpenAccountField &data) {
  out << "CThostFtdcReqOpenAccountField" << endl
      << "业务功能码, "
      << "TradeCode, " << data.TradeCode << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分支机构代码, "
      << "BankBranchID, " << data.BankBranchID << endl
      << "期商代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "期商分支机构代码, "
      << "BrokerBranchID, " << data.BrokerBranchID << endl
      << "交易日期, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "银行流水号, "
      << "BankSerial, " << data.BankSerial << endl
      << "交易系统日期 , "
      << "TradingDay, " << data.TradingDay << endl
      << "银期平台消息流水号, "
      << "PlateSerial, " << data.PlateSerial << endl
      << "最后分片标志, "
      << "LastFragment, " << data.LastFragment << endl
      << "会话号, "
      << "SessionID, " << data.SessionID << endl
      << "客户姓名, "
      << "CustomerName, " << data.CustomerName << endl
      << "证件类型, "
      << "IdCardType, " << data.IdCardType << endl
      << "证件号码, "
      << "IdentifiedCardNo, " << data.IdentifiedCardNo << endl
      << "性别, "
      << "Gender, " << data.Gender << endl
      << "国家代码, "
      << "CountryCode, " << data.CountryCode << endl
      << "客户类型, "
      << "CustType, " << data.CustType << endl
      << "地址, "
      << "Address, " << data.Address << endl
      << "邮编, "
      << "ZipCode, " << data.ZipCode << endl
      << "电话号码, "
      << "Telephone, " << data.Telephone << endl
      << "手机, "
      << "MobilePhone, " << data.MobilePhone << endl
      << "传真, "
      << "Fax, " << data.Fax << endl
      << "电子邮件, "
      << "EMail, " << data.EMail << endl
      << "资金账户状态, "
      << "MoneyAccountStatus, " << data.MoneyAccountStatus << endl
      << "银行帐号, "
      << "BankAccount, " << data.BankAccount << endl
      << "银行密码, "
      << "BankPassWord, " << data.BankPassWord << endl
      << "投资者帐号, "
      << "AccountID, " << data.AccountID << endl
      << "期货密码, "
      << "Password, " << data.Password << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "验证客户证件号码标志, "
      << "VerifyCertNoFlag, " << data.VerifyCertNoFlag << endl
      << "币种代码, "
      << "CurrencyID, " << data.CurrencyID << endl
      << "汇钞标志, "
      << "CashExchangeCode, " << data.CashExchangeCode << endl
      << "摘要, "
      << "Digest, " << data.Digest << endl
      << "银行帐号类型, "
      << "BankAccType, " << data.BankAccType << endl
      << "渠道标志, "
      << "DeviceID, " << data.DeviceID << endl
      << "期货单位帐号类型, "
      << "BankSecuAccType, " << data.BankSecuAccType << endl
      << "期货公司银行编码, "
      << "BrokerIDByBank, " << data.BrokerIDByBank << endl
      << "期货单位帐号, "
      << "BankSecuAcc, " << data.BankSecuAcc << endl
      << "银行密码标志, "
      << "BankPwdFlag, " << data.BankPwdFlag << endl
      << "期货资金密码核对标志, "
      << "SecuPwdFlag, " << data.SecuPwdFlag << endl
      << "交易柜员, "
      << "OperNo, " << data.OperNo << endl
      << "交易ID, "
      << "TID, " << data.TID << endl;
  return out;
}

/* 转帐销户请求 */
ostream &operator<<(ostream &out, CThostFtdcReqCancelAccountField &data) {
  out << "CThostFtdcReqCancelAccountField" << endl
      << "业务功能码, "
      << "TradeCode, " << data.TradeCode << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分支机构代码, "
      << "BankBranchID, " << data.BankBranchID << endl
      << "期商代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "期商分支机构代码, "
      << "BrokerBranchID, " << data.BrokerBranchID << endl
      << "交易日期, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "银行流水号, "
      << "BankSerial, " << data.BankSerial << endl
      << "交易系统日期 , "
      << "TradingDay, " << data.TradingDay << endl
      << "银期平台消息流水号, "
      << "PlateSerial, " << data.PlateSerial << endl
      << "最后分片标志, "
      << "LastFragment, " << data.LastFragment << endl
      << "会话号, "
      << "SessionID, " << data.SessionID << endl
      << "客户姓名, "
      << "CustomerName, " << data.CustomerName << endl
      << "证件类型, "
      << "IdCardType, " << data.IdCardType << endl
      << "证件号码, "
      << "IdentifiedCardNo, " << data.IdentifiedCardNo << endl
      << "性别, "
      << "Gender, " << data.Gender << endl
      << "国家代码, "
      << "CountryCode, " << data.CountryCode << endl
      << "客户类型, "
      << "CustType, " << data.CustType << endl
      << "地址, "
      << "Address, " << data.Address << endl
      << "邮编, "
      << "ZipCode, " << data.ZipCode << endl
      << "电话号码, "
      << "Telephone, " << data.Telephone << endl
      << "手机, "
      << "MobilePhone, " << data.MobilePhone << endl
      << "传真, "
      << "Fax, " << data.Fax << endl
      << "电子邮件, "
      << "EMail, " << data.EMail << endl
      << "资金账户状态, "
      << "MoneyAccountStatus, " << data.MoneyAccountStatus << endl
      << "银行帐号, "
      << "BankAccount, " << data.BankAccount << endl
      << "银行密码, "
      << "BankPassWord, " << data.BankPassWord << endl
      << "投资者帐号, "
      << "AccountID, " << data.AccountID << endl
      << "期货密码, "
      << "Password, " << data.Password << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "验证客户证件号码标志, "
      << "VerifyCertNoFlag, " << data.VerifyCertNoFlag << endl
      << "币种代码, "
      << "CurrencyID, " << data.CurrencyID << endl
      << "汇钞标志, "
      << "CashExchangeCode, " << data.CashExchangeCode << endl
      << "摘要, "
      << "Digest, " << data.Digest << endl
      << "银行帐号类型, "
      << "BankAccType, " << data.BankAccType << endl
      << "渠道标志, "
      << "DeviceID, " << data.DeviceID << endl
      << "期货单位帐号类型, "
      << "BankSecuAccType, " << data.BankSecuAccType << endl
      << "期货公司银行编码, "
      << "BrokerIDByBank, " << data.BrokerIDByBank << endl
      << "期货单位帐号, "
      << "BankSecuAcc, " << data.BankSecuAcc << endl
      << "银行密码标志, "
      << "BankPwdFlag, " << data.BankPwdFlag << endl
      << "期货资金密码核对标志, "
      << "SecuPwdFlag, " << data.SecuPwdFlag << endl
      << "交易柜员, "
      << "OperNo, " << data.OperNo << endl
      << "交易ID, "
      << "TID, " << data.TID << endl;
  return out;
}

/* 变更银行账户请求 */
ostream &operator<<(ostream &out, CThostFtdcReqChangeAccountField &data) {
  out << "CThostFtdcReqChangeAccountField" << endl
      << "业务功能码, "
      << "TradeCode, " << data.TradeCode << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分支机构代码, "
      << "BankBranchID, " << data.BankBranchID << endl
      << "期商代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "期商分支机构代码, "
      << "BrokerBranchID, " << data.BrokerBranchID << endl
      << "交易日期, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "银行流水号, "
      << "BankSerial, " << data.BankSerial << endl
      << "交易系统日期 , "
      << "TradingDay, " << data.TradingDay << endl
      << "银期平台消息流水号, "
      << "PlateSerial, " << data.PlateSerial << endl
      << "最后分片标志, "
      << "LastFragment, " << data.LastFragment << endl
      << "会话号, "
      << "SessionID, " << data.SessionID << endl
      << "客户姓名, "
      << "CustomerName, " << data.CustomerName << endl
      << "证件类型, "
      << "IdCardType, " << data.IdCardType << endl
      << "证件号码, "
      << "IdentifiedCardNo, " << data.IdentifiedCardNo << endl
      << "性别, "
      << "Gender, " << data.Gender << endl
      << "国家代码, "
      << "CountryCode, " << data.CountryCode << endl
      << "客户类型, "
      << "CustType, " << data.CustType << endl
      << "地址, "
      << "Address, " << data.Address << endl
      << "邮编, "
      << "ZipCode, " << data.ZipCode << endl
      << "电话号码, "
      << "Telephone, " << data.Telephone << endl
      << "手机, "
      << "MobilePhone, " << data.MobilePhone << endl
      << "传真, "
      << "Fax, " << data.Fax << endl
      << "电子邮件, "
      << "EMail, " << data.EMail << endl
      << "资金账户状态, "
      << "MoneyAccountStatus, " << data.MoneyAccountStatus << endl
      << "银行帐号, "
      << "BankAccount, " << data.BankAccount << endl
      << "银行密码, "
      << "BankPassWord, " << data.BankPassWord << endl
      << "新银行帐号, "
      << "NewBankAccount, " << data.NewBankAccount << endl
      << "新银行密码, "
      << "NewBankPassWord, " << data.NewBankPassWord << endl
      << "投资者帐号, "
      << "AccountID, " << data.AccountID << endl
      << "期货密码, "
      << "Password, " << data.Password << endl
      << "银行帐号类型, "
      << "BankAccType, " << data.BankAccType << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "验证客户证件号码标志, "
      << "VerifyCertNoFlag, " << data.VerifyCertNoFlag << endl
      << "币种代码, "
      << "CurrencyID, " << data.CurrencyID << endl
      << "期货公司银行编码, "
      << "BrokerIDByBank, " << data.BrokerIDByBank << endl
      << "银行密码标志, "
      << "BankPwdFlag, " << data.BankPwdFlag << endl
      << "期货资金密码核对标志, "
      << "SecuPwdFlag, " << data.SecuPwdFlag << endl
      << "交易ID, "
      << "TID, " << data.TID << endl
      << "摘要, "
      << "Digest, " << data.Digest << endl;
  return out;
}

/* 转账请求 */
ostream &operator<<(ostream &out, CThostFtdcReqTransferField &data) {
  out << "CThostFtdcReqTransferField" << endl
      << "业务功能码, "
      << "TradeCode, " << data.TradeCode << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分支机构代码, "
      << "BankBranchID, " << data.BankBranchID << endl
      << "期商代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "期商分支机构代码, "
      << "BrokerBranchID, " << data.BrokerBranchID << endl
      << "交易日期, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "银行流水号, "
      << "BankSerial, " << data.BankSerial << endl
      << "交易系统日期 , "
      << "TradingDay, " << data.TradingDay << endl
      << "银期平台消息流水号, "
      << "PlateSerial, " << data.PlateSerial << endl
      << "最后分片标志, "
      << "LastFragment, " << data.LastFragment << endl
      << "会话号, "
      << "SessionID, " << data.SessionID << endl
      << "客户姓名, "
      << "CustomerName, " << data.CustomerName << endl
      << "证件类型, "
      << "IdCardType, " << data.IdCardType << endl
      << "证件号码, "
      << "IdentifiedCardNo, " << data.IdentifiedCardNo << endl
      << "客户类型, "
      << "CustType, " << data.CustType << endl
      << "银行帐号, "
      << "BankAccount, " << data.BankAccount << endl
      << "银行密码, "
      << "BankPassWord, " << data.BankPassWord << endl
      << "投资者帐号, "
      << "AccountID, " << data.AccountID << endl
      << "期货密码, "
      << "Password, " << data.Password << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "期货公司流水号, "
      << "FutureSerial, " << data.FutureSerial << endl
      << "用户标识, "
      << "UserID, " << data.UserID << endl
      << "验证客户证件号码标志, "
      << "VerifyCertNoFlag, " << data.VerifyCertNoFlag << endl
      << "币种代码, "
      << "CurrencyID, " << data.CurrencyID << endl
      << "转帐金额, "
      << "TradeAmount, " << data.TradeAmount << endl
      << "期货可取金额, "
      << "FutureFetchAmount, " << data.FutureFetchAmount << endl
      << "费用支付标志, "
      << "FeePayFlag, " << data.FeePayFlag << endl
      << "应收客户费用, "
      << "CustFee, " << data.CustFee << endl
      << "应收期货公司费用, "
      << "BrokerFee, " << data.BrokerFee << endl
      << "发送方给接收方的消息, "
      << "Message, " << data.Message << endl
      << "摘要, "
      << "Digest, " << data.Digest << endl
      << "银行帐号类型, "
      << "BankAccType, " << data.BankAccType << endl
      << "渠道标志, "
      << "DeviceID, " << data.DeviceID << endl
      << "期货单位帐号类型, "
      << "BankSecuAccType, " << data.BankSecuAccType << endl
      << "期货公司银行编码, "
      << "BrokerIDByBank, " << data.BrokerIDByBank << endl
      << "期货单位帐号, "
      << "BankSecuAcc, " << data.BankSecuAcc << endl
      << "银行密码标志, "
      << "BankPwdFlag, " << data.BankPwdFlag << endl
      << "期货资金密码核对标志, "
      << "SecuPwdFlag, " << data.SecuPwdFlag << endl
      << "交易柜员, "
      << "OperNo, " << data.OperNo << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "交易ID, "
      << "TID, " << data.TID << endl
      << "转账交易状态, "
      << "TransferStatus, " << data.TransferStatus << endl;
  return out;
}

/* 银行发起银行资金转期货响应 */
ostream &operator<<(ostream &out, CThostFtdcRspTransferField &data) {
  out << "CThostFtdcRspTransferField" << endl
      << "业务功能码, "
      << "TradeCode, " << data.TradeCode << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分支机构代码, "
      << "BankBranchID, " << data.BankBranchID << endl
      << "期商代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "期商分支机构代码, "
      << "BrokerBranchID, " << data.BrokerBranchID << endl
      << "交易日期, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "银行流水号, "
      << "BankSerial, " << data.BankSerial << endl
      << "交易系统日期 , "
      << "TradingDay, " << data.TradingDay << endl
      << "银期平台消息流水号, "
      << "PlateSerial, " << data.PlateSerial << endl
      << "最后分片标志, "
      << "LastFragment, " << data.LastFragment << endl
      << "会话号, "
      << "SessionID, " << data.SessionID << endl
      << "客户姓名, "
      << "CustomerName, " << data.CustomerName << endl
      << "证件类型, "
      << "IdCardType, " << data.IdCardType << endl
      << "证件号码, "
      << "IdentifiedCardNo, " << data.IdentifiedCardNo << endl
      << "客户类型, "
      << "CustType, " << data.CustType << endl
      << "银行帐号, "
      << "BankAccount, " << data.BankAccount << endl
      << "银行密码, "
      << "BankPassWord, " << data.BankPassWord << endl
      << "投资者帐号, "
      << "AccountID, " << data.AccountID << endl
      << "期货密码, "
      << "Password, " << data.Password << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "期货公司流水号, "
      << "FutureSerial, " << data.FutureSerial << endl
      << "用户标识, "
      << "UserID, " << data.UserID << endl
      << "验证客户证件号码标志, "
      << "VerifyCertNoFlag, " << data.VerifyCertNoFlag << endl
      << "币种代码, "
      << "CurrencyID, " << data.CurrencyID << endl
      << "转帐金额, "
      << "TradeAmount, " << data.TradeAmount << endl
      << "期货可取金额, "
      << "FutureFetchAmount, " << data.FutureFetchAmount << endl
      << "费用支付标志, "
      << "FeePayFlag, " << data.FeePayFlag << endl
      << "应收客户费用, "
      << "CustFee, " << data.CustFee << endl
      << "应收期货公司费用, "
      << "BrokerFee, " << data.BrokerFee << endl
      << "发送方给接收方的消息, "
      << "Message, " << data.Message << endl
      << "摘要, "
      << "Digest, " << data.Digest << endl
      << "银行帐号类型, "
      << "BankAccType, " << data.BankAccType << endl
      << "渠道标志, "
      << "DeviceID, " << data.DeviceID << endl
      << "期货单位帐号类型, "
      << "BankSecuAccType, " << data.BankSecuAccType << endl
      << "期货公司银行编码, "
      << "BrokerIDByBank, " << data.BrokerIDByBank << endl
      << "期货单位帐号, "
      << "BankSecuAcc, " << data.BankSecuAcc << endl
      << "银行密码标志, "
      << "BankPwdFlag, " << data.BankPwdFlag << endl
      << "期货资金密码核对标志, "
      << "SecuPwdFlag, " << data.SecuPwdFlag << endl
      << "交易柜员, "
      << "OperNo, " << data.OperNo << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "交易ID, "
      << "TID, " << data.TID << endl
      << "转账交易状态, "
      << "TransferStatus, " << data.TransferStatus << endl
      << "错误代码, "
      << "ErrorID, " << data.ErrorID << endl
      << "错误信息, "
      << "ErrorMsg, " << data.ErrorMsg << endl;
  return out;
}

/* 冲正请求 */
ostream &operator<<(ostream &out, CThostFtdcReqRepealField &data) {
  out << "CThostFtdcReqRepealField" << endl
      << "冲正时间间隔, "
      << "RepealTimeInterval, " << data.RepealTimeInterval << endl
      << "已经冲正次数, "
      << "RepealedTimes, " << data.RepealedTimes << endl
      << "银行冲正标志, "
      << "BankRepealFlag, " << data.BankRepealFlag << endl
      << "期商冲正标志, "
      << "BrokerRepealFlag, " << data.BrokerRepealFlag << endl
      << "被冲正平台流水号, "
      << "PlateRepealSerial, " << data.PlateRepealSerial << endl
      << "被冲正银行流水号, "
      << "BankRepealSerial, " << data.BankRepealSerial << endl
      << "被冲正期货流水号, "
      << "FutureRepealSerial, " << data.FutureRepealSerial << endl
      << "业务功能码, "
      << "TradeCode, " << data.TradeCode << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分支机构代码, "
      << "BankBranchID, " << data.BankBranchID << endl
      << "期商代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "期商分支机构代码, "
      << "BrokerBranchID, " << data.BrokerBranchID << endl
      << "交易日期, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "银行流水号, "
      << "BankSerial, " << data.BankSerial << endl
      << "交易系统日期 , "
      << "TradingDay, " << data.TradingDay << endl
      << "银期平台消息流水号, "
      << "PlateSerial, " << data.PlateSerial << endl
      << "最后分片标志, "
      << "LastFragment, " << data.LastFragment << endl
      << "会话号, "
      << "SessionID, " << data.SessionID << endl
      << "客户姓名, "
      << "CustomerName, " << data.CustomerName << endl
      << "证件类型, "
      << "IdCardType, " << data.IdCardType << endl
      << "证件号码, "
      << "IdentifiedCardNo, " << data.IdentifiedCardNo << endl
      << "客户类型, "
      << "CustType, " << data.CustType << endl
      << "银行帐号, "
      << "BankAccount, " << data.BankAccount << endl
      << "银行密码, "
      << "BankPassWord, " << data.BankPassWord << endl
      << "投资者帐号, "
      << "AccountID, " << data.AccountID << endl
      << "期货密码, "
      << "Password, " << data.Password << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "期货公司流水号, "
      << "FutureSerial, " << data.FutureSerial << endl
      << "用户标识, "
      << "UserID, " << data.UserID << endl
      << "验证客户证件号码标志, "
      << "VerifyCertNoFlag, " << data.VerifyCertNoFlag << endl
      << "币种代码, "
      << "CurrencyID, " << data.CurrencyID << endl
      << "转帐金额, "
      << "TradeAmount, " << data.TradeAmount << endl
      << "期货可取金额, "
      << "FutureFetchAmount, " << data.FutureFetchAmount << endl
      << "费用支付标志, "
      << "FeePayFlag, " << data.FeePayFlag << endl
      << "应收客户费用, "
      << "CustFee, " << data.CustFee << endl
      << "应收期货公司费用, "
      << "BrokerFee, " << data.BrokerFee << endl
      << "发送方给接收方的消息, "
      << "Message, " << data.Message << endl
      << "摘要, "
      << "Digest, " << data.Digest << endl
      << "银行帐号类型, "
      << "BankAccType, " << data.BankAccType << endl
      << "渠道标志, "
      << "DeviceID, " << data.DeviceID << endl
      << "期货单位帐号类型, "
      << "BankSecuAccType, " << data.BankSecuAccType << endl
      << "期货公司银行编码, "
      << "BrokerIDByBank, " << data.BrokerIDByBank << endl
      << "期货单位帐号, "
      << "BankSecuAcc, " << data.BankSecuAcc << endl
      << "银行密码标志, "
      << "BankPwdFlag, " << data.BankPwdFlag << endl
      << "期货资金密码核对标志, "
      << "SecuPwdFlag, " << data.SecuPwdFlag << endl
      << "交易柜员, "
      << "OperNo, " << data.OperNo << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "交易ID, "
      << "TID, " << data.TID << endl
      << "转账交易状态, "
      << "TransferStatus, " << data.TransferStatus << endl;
  return out;
}

/* 冲正响应 */
ostream &operator<<(ostream &out, CThostFtdcRspRepealField &data) {
  out << "CThostFtdcRspRepealField" << endl
      << "冲正时间间隔, "
      << "RepealTimeInterval, " << data.RepealTimeInterval << endl
      << "已经冲正次数, "
      << "RepealedTimes, " << data.RepealedTimes << endl
      << "银行冲正标志, "
      << "BankRepealFlag, " << data.BankRepealFlag << endl
      << "期商冲正标志, "
      << "BrokerRepealFlag, " << data.BrokerRepealFlag << endl
      << "被冲正平台流水号, "
      << "PlateRepealSerial, " << data.PlateRepealSerial << endl
      << "被冲正银行流水号, "
      << "BankRepealSerial, " << data.BankRepealSerial << endl
      << "被冲正期货流水号, "
      << "FutureRepealSerial, " << data.FutureRepealSerial << endl
      << "业务功能码, "
      << "TradeCode, " << data.TradeCode << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分支机构代码, "
      << "BankBranchID, " << data.BankBranchID << endl
      << "期商代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "期商分支机构代码, "
      << "BrokerBranchID, " << data.BrokerBranchID << endl
      << "交易日期, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "银行流水号, "
      << "BankSerial, " << data.BankSerial << endl
      << "交易系统日期 , "
      << "TradingDay, " << data.TradingDay << endl
      << "银期平台消息流水号, "
      << "PlateSerial, " << data.PlateSerial << endl
      << "最后分片标志, "
      << "LastFragment, " << data.LastFragment << endl
      << "会话号, "
      << "SessionID, " << data.SessionID << endl
      << "客户姓名, "
      << "CustomerName, " << data.CustomerName << endl
      << "证件类型, "
      << "IdCardType, " << data.IdCardType << endl
      << "证件号码, "
      << "IdentifiedCardNo, " << data.IdentifiedCardNo << endl
      << "客户类型, "
      << "CustType, " << data.CustType << endl
      << "银行帐号, "
      << "BankAccount, " << data.BankAccount << endl
      << "银行密码, "
      << "BankPassWord, " << data.BankPassWord << endl
      << "投资者帐号, "
      << "AccountID, " << data.AccountID << endl
      << "期货密码, "
      << "Password, " << data.Password << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "期货公司流水号, "
      << "FutureSerial, " << data.FutureSerial << endl
      << "用户标识, "
      << "UserID, " << data.UserID << endl
      << "验证客户证件号码标志, "
      << "VerifyCertNoFlag, " << data.VerifyCertNoFlag << endl
      << "币种代码, "
      << "CurrencyID, " << data.CurrencyID << endl
      << "转帐金额, "
      << "TradeAmount, " << data.TradeAmount << endl
      << "期货可取金额, "
      << "FutureFetchAmount, " << data.FutureFetchAmount << endl
      << "费用支付标志, "
      << "FeePayFlag, " << data.FeePayFlag << endl
      << "应收客户费用, "
      << "CustFee, " << data.CustFee << endl
      << "应收期货公司费用, "
      << "BrokerFee, " << data.BrokerFee << endl
      << "发送方给接收方的消息, "
      << "Message, " << data.Message << endl
      << "摘要, "
      << "Digest, " << data.Digest << endl
      << "银行帐号类型, "
      << "BankAccType, " << data.BankAccType << endl
      << "渠道标志, "
      << "DeviceID, " << data.DeviceID << endl
      << "期货单位帐号类型, "
      << "BankSecuAccType, " << data.BankSecuAccType << endl
      << "期货公司银行编码, "
      << "BrokerIDByBank, " << data.BrokerIDByBank << endl
      << "期货单位帐号, "
      << "BankSecuAcc, " << data.BankSecuAcc << endl
      << "银行密码标志, "
      << "BankPwdFlag, " << data.BankPwdFlag << endl
      << "期货资金密码核对标志, "
      << "SecuPwdFlag, " << data.SecuPwdFlag << endl
      << "交易柜员, "
      << "OperNo, " << data.OperNo << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "交易ID, "
      << "TID, " << data.TID << endl
      << "转账交易状态, "
      << "TransferStatus, " << data.TransferStatus << endl
      << "错误代码, "
      << "ErrorID, " << data.ErrorID << endl
      << "错误信息, "
      << "ErrorMsg, " << data.ErrorMsg << endl;
  return out;
}

/* 查询账户信息请求 */
ostream &operator<<(ostream &out, CThostFtdcReqQueryAccountField &data) {
  out << "CThostFtdcReqQueryAccountField" << endl
      << "业务功能码, "
      << "TradeCode, " << data.TradeCode << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分支机构代码, "
      << "BankBranchID, " << data.BankBranchID << endl
      << "期商代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "期商分支机构代码, "
      << "BrokerBranchID, " << data.BrokerBranchID << endl
      << "交易日期, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "银行流水号, "
      << "BankSerial, " << data.BankSerial << endl
      << "交易系统日期 , "
      << "TradingDay, " << data.TradingDay << endl
      << "银期平台消息流水号, "
      << "PlateSerial, " << data.PlateSerial << endl
      << "最后分片标志, "
      << "LastFragment, " << data.LastFragment << endl
      << "会话号, "
      << "SessionID, " << data.SessionID << endl
      << "客户姓名, "
      << "CustomerName, " << data.CustomerName << endl
      << "证件类型, "
      << "IdCardType, " << data.IdCardType << endl
      << "证件号码, "
      << "IdentifiedCardNo, " << data.IdentifiedCardNo << endl
      << "客户类型, "
      << "CustType, " << data.CustType << endl
      << "银行帐号, "
      << "BankAccount, " << data.BankAccount << endl
      << "银行密码, "
      << "BankPassWord, " << data.BankPassWord << endl
      << "投资者帐号, "
      << "AccountID, " << data.AccountID << endl
      << "期货密码, "
      << "Password, " << data.Password << endl
      << "期货公司流水号, "
      << "FutureSerial, " << data.FutureSerial << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "用户标识, "
      << "UserID, " << data.UserID << endl
      << "验证客户证件号码标志, "
      << "VerifyCertNoFlag, " << data.VerifyCertNoFlag << endl
      << "币种代码, "
      << "CurrencyID, " << data.CurrencyID << endl
      << "摘要, "
      << "Digest, " << data.Digest << endl
      << "银行帐号类型, "
      << "BankAccType, " << data.BankAccType << endl
      << "渠道标志, "
      << "DeviceID, " << data.DeviceID << endl
      << "期货单位帐号类型, "
      << "BankSecuAccType, " << data.BankSecuAccType << endl
      << "期货公司银行编码, "
      << "BrokerIDByBank, " << data.BrokerIDByBank << endl
      << "期货单位帐号, "
      << "BankSecuAcc, " << data.BankSecuAcc << endl
      << "银行密码标志, "
      << "BankPwdFlag, " << data.BankPwdFlag << endl
      << "期货资金密码核对标志, "
      << "SecuPwdFlag, " << data.SecuPwdFlag << endl
      << "交易柜员, "
      << "OperNo, " << data.OperNo << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "交易ID, "
      << "TID, " << data.TID << endl;
  return out;
}

/* 查询账户信息响应 */
ostream &operator<<(ostream &out, CThostFtdcRspQueryAccountField &data) {
  out << "CThostFtdcRspQueryAccountField" << endl
      << "业务功能码, "
      << "TradeCode, " << data.TradeCode << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分支机构代码, "
      << "BankBranchID, " << data.BankBranchID << endl
      << "期商代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "期商分支机构代码, "
      << "BrokerBranchID, " << data.BrokerBranchID << endl
      << "交易日期, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "银行流水号, "
      << "BankSerial, " << data.BankSerial << endl
      << "交易系统日期 , "
      << "TradingDay, " << data.TradingDay << endl
      << "银期平台消息流水号, "
      << "PlateSerial, " << data.PlateSerial << endl
      << "最后分片标志, "
      << "LastFragment, " << data.LastFragment << endl
      << "会话号, "
      << "SessionID, " << data.SessionID << endl
      << "客户姓名, "
      << "CustomerName, " << data.CustomerName << endl
      << "证件类型, "
      << "IdCardType, " << data.IdCardType << endl
      << "证件号码, "
      << "IdentifiedCardNo, " << data.IdentifiedCardNo << endl
      << "客户类型, "
      << "CustType, " << data.CustType << endl
      << "银行帐号, "
      << "BankAccount, " << data.BankAccount << endl
      << "银行密码, "
      << "BankPassWord, " << data.BankPassWord << endl
      << "投资者帐号, "
      << "AccountID, " << data.AccountID << endl
      << "期货密码, "
      << "Password, " << data.Password << endl
      << "期货公司流水号, "
      << "FutureSerial, " << data.FutureSerial << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "用户标识, "
      << "UserID, " << data.UserID << endl
      << "验证客户证件号码标志, "
      << "VerifyCertNoFlag, " << data.VerifyCertNoFlag << endl
      << "币种代码, "
      << "CurrencyID, " << data.CurrencyID << endl
      << "摘要, "
      << "Digest, " << data.Digest << endl
      << "银行帐号类型, "
      << "BankAccType, " << data.BankAccType << endl
      << "渠道标志, "
      << "DeviceID, " << data.DeviceID << endl
      << "期货单位帐号类型, "
      << "BankSecuAccType, " << data.BankSecuAccType << endl
      << "期货公司银行编码, "
      << "BrokerIDByBank, " << data.BrokerIDByBank << endl
      << "期货单位帐号, "
      << "BankSecuAcc, " << data.BankSecuAcc << endl
      << "银行密码标志, "
      << "BankPwdFlag, " << data.BankPwdFlag << endl
      << "期货资金密码核对标志, "
      << "SecuPwdFlag, " << data.SecuPwdFlag << endl
      << "交易柜员, "
      << "OperNo, " << data.OperNo << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "交易ID, "
      << "TID, " << data.TID << endl
      << "银行可用金额, "
      << "BankUseAmount, " << data.BankUseAmount << endl
      << "银行可取金额, "
      << "BankFetchAmount, " << data.BankFetchAmount << endl;
  return out;
}

/* 期商签到签退 */
ostream &operator<<(ostream &out, CThostFtdcFutureSignIOField &data) {
  out << "CThostFtdcFutureSignIOField" << endl
      << "业务功能码, "
      << "TradeCode, " << data.TradeCode << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分支机构代码, "
      << "BankBranchID, " << data.BankBranchID << endl
      << "期商代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "期商分支机构代码, "
      << "BrokerBranchID, " << data.BrokerBranchID << endl
      << "交易日期, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "银行流水号, "
      << "BankSerial, " << data.BankSerial << endl
      << "交易系统日期 , "
      << "TradingDay, " << data.TradingDay << endl
      << "银期平台消息流水号, "
      << "PlateSerial, " << data.PlateSerial << endl
      << "最后分片标志, "
      << "LastFragment, " << data.LastFragment << endl
      << "会话号, "
      << "SessionID, " << data.SessionID << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "用户标识, "
      << "UserID, " << data.UserID << endl
      << "摘要, "
      << "Digest, " << data.Digest << endl
      << "币种代码, "
      << "CurrencyID, " << data.CurrencyID << endl
      << "渠道标志, "
      << "DeviceID, " << data.DeviceID << endl
      << "期货公司银行编码, "
      << "BrokerIDByBank, " << data.BrokerIDByBank << endl
      << "交易柜员, "
      << "OperNo, " << data.OperNo << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "交易ID, "
      << "TID, " << data.TID << endl;
  return out;
}

/* 期商签到响应 */
ostream &operator<<(ostream &out, CThostFtdcRspFutureSignInField &data) {
  out << "CThostFtdcRspFutureSignInField" << endl
      << "业务功能码, "
      << "TradeCode, " << data.TradeCode << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分支机构代码, "
      << "BankBranchID, " << data.BankBranchID << endl
      << "期商代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "期商分支机构代码, "
      << "BrokerBranchID, " << data.BrokerBranchID << endl
      << "交易日期, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "银行流水号, "
      << "BankSerial, " << data.BankSerial << endl
      << "交易系统日期 , "
      << "TradingDay, " << data.TradingDay << endl
      << "银期平台消息流水号, "
      << "PlateSerial, " << data.PlateSerial << endl
      << "最后分片标志, "
      << "LastFragment, " << data.LastFragment << endl
      << "会话号, "
      << "SessionID, " << data.SessionID << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "用户标识, "
      << "UserID, " << data.UserID << endl
      << "摘要, "
      << "Digest, " << data.Digest << endl
      << "币种代码, "
      << "CurrencyID, " << data.CurrencyID << endl
      << "渠道标志, "
      << "DeviceID, " << data.DeviceID << endl
      << "期货公司银行编码, "
      << "BrokerIDByBank, " << data.BrokerIDByBank << endl
      << "交易柜员, "
      << "OperNo, " << data.OperNo << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "交易ID, "
      << "TID, " << data.TID << endl
      << "错误代码, "
      << "ErrorID, " << data.ErrorID << endl
      << "错误信息, "
      << "ErrorMsg, " << data.ErrorMsg << endl
      << "PIN密钥, "
      << "PinKey, " << data.PinKey << endl
      << "MAC密钥, "
      << "MacKey, " << data.MacKey << endl;
  return out;
}

/* 期商签退请求 */
ostream &operator<<(ostream &out, CThostFtdcReqFutureSignOutField &data) {
  out << "CThostFtdcReqFutureSignOutField" << endl
      << "业务功能码, "
      << "TradeCode, " << data.TradeCode << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分支机构代码, "
      << "BankBranchID, " << data.BankBranchID << endl
      << "期商代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "期商分支机构代码, "
      << "BrokerBranchID, " << data.BrokerBranchID << endl
      << "交易日期, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "银行流水号, "
      << "BankSerial, " << data.BankSerial << endl
      << "交易系统日期 , "
      << "TradingDay, " << data.TradingDay << endl
      << "银期平台消息流水号, "
      << "PlateSerial, " << data.PlateSerial << endl
      << "最后分片标志, "
      << "LastFragment, " << data.LastFragment << endl
      << "会话号, "
      << "SessionID, " << data.SessionID << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "用户标识, "
      << "UserID, " << data.UserID << endl
      << "摘要, "
      << "Digest, " << data.Digest << endl
      << "币种代码, "
      << "CurrencyID, " << data.CurrencyID << endl
      << "渠道标志, "
      << "DeviceID, " << data.DeviceID << endl
      << "期货公司银行编码, "
      << "BrokerIDByBank, " << data.BrokerIDByBank << endl
      << "交易柜员, "
      << "OperNo, " << data.OperNo << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "交易ID, "
      << "TID, " << data.TID << endl;
  return out;
}

/* 期商签退响应 */
ostream &operator<<(ostream &out, CThostFtdcRspFutureSignOutField &data) {
  out << "CThostFtdcRspFutureSignOutField" << endl
      << "业务功能码, "
      << "TradeCode, " << data.TradeCode << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分支机构代码, "
      << "BankBranchID, " << data.BankBranchID << endl
      << "期商代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "期商分支机构代码, "
      << "BrokerBranchID, " << data.BrokerBranchID << endl
      << "交易日期, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "银行流水号, "
      << "BankSerial, " << data.BankSerial << endl
      << "交易系统日期 , "
      << "TradingDay, " << data.TradingDay << endl
      << "银期平台消息流水号, "
      << "PlateSerial, " << data.PlateSerial << endl
      << "最后分片标志, "
      << "LastFragment, " << data.LastFragment << endl
      << "会话号, "
      << "SessionID, " << data.SessionID << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "用户标识, "
      << "UserID, " << data.UserID << endl
      << "摘要, "
      << "Digest, " << data.Digest << endl
      << "币种代码, "
      << "CurrencyID, " << data.CurrencyID << endl
      << "渠道标志, "
      << "DeviceID, " << data.DeviceID << endl
      << "期货公司银行编码, "
      << "BrokerIDByBank, " << data.BrokerIDByBank << endl
      << "交易柜员, "
      << "OperNo, " << data.OperNo << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "交易ID, "
      << "TID, " << data.TID << endl
      << "错误代码, "
      << "ErrorID, " << data.ErrorID << endl
      << "错误信息, "
      << "ErrorMsg, " << data.ErrorMsg << endl;
  return out;
}

/* 查询指定流水号的交易结果请求 */
ostream &operator<<(ostream &out,
                    CThostFtdcReqQueryTradeResultBySerialField &data) {
  out << "CThostFtdcReqQueryTradeResultBySerialField" << endl
      << "业务功能码, "
      << "TradeCode, " << data.TradeCode << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分支机构代码, "
      << "BankBranchID, " << data.BankBranchID << endl
      << "期商代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "期商分支机构代码, "
      << "BrokerBranchID, " << data.BrokerBranchID << endl
      << "交易日期, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "银行流水号, "
      << "BankSerial, " << data.BankSerial << endl
      << "交易系统日期 , "
      << "TradingDay, " << data.TradingDay << endl
      << "银期平台消息流水号, "
      << "PlateSerial, " << data.PlateSerial << endl
      << "最后分片标志, "
      << "LastFragment, " << data.LastFragment << endl
      << "会话号, "
      << "SessionID, " << data.SessionID << endl
      << "流水号, "
      << "Reference, " << data.Reference << endl
      << "本流水号发布者的机构类型, "
      << "RefrenceIssureType, " << data.RefrenceIssureType << endl
      << "本流水号发布者机构编码, "
      << "RefrenceIssure, " << data.RefrenceIssure << endl
      << "客户姓名, "
      << "CustomerName, " << data.CustomerName << endl
      << "证件类型, "
      << "IdCardType, " << data.IdCardType << endl
      << "证件号码, "
      << "IdentifiedCardNo, " << data.IdentifiedCardNo << endl
      << "客户类型, "
      << "CustType, " << data.CustType << endl
      << "银行帐号, "
      << "BankAccount, " << data.BankAccount << endl
      << "银行密码, "
      << "BankPassWord, " << data.BankPassWord << endl
      << "投资者帐号, "
      << "AccountID, " << data.AccountID << endl
      << "期货密码, "
      << "Password, " << data.Password << endl
      << "币种代码, "
      << "CurrencyID, " << data.CurrencyID << endl
      << "转帐金额, "
      << "TradeAmount, " << data.TradeAmount << endl
      << "摘要, "
      << "Digest, " << data.Digest << endl;
  return out;
}

/* 查询指定流水号的交易结果响应 */
ostream &operator<<(ostream &out,
                    CThostFtdcRspQueryTradeResultBySerialField &data) {
  out << "CThostFtdcRspQueryTradeResultBySerialField" << endl
      << "业务功能码, "
      << "TradeCode, " << data.TradeCode << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分支机构代码, "
      << "BankBranchID, " << data.BankBranchID << endl
      << "期商代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "期商分支机构代码, "
      << "BrokerBranchID, " << data.BrokerBranchID << endl
      << "交易日期, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "银行流水号, "
      << "BankSerial, " << data.BankSerial << endl
      << "交易系统日期 , "
      << "TradingDay, " << data.TradingDay << endl
      << "银期平台消息流水号, "
      << "PlateSerial, " << data.PlateSerial << endl
      << "最后分片标志, "
      << "LastFragment, " << data.LastFragment << endl
      << "会话号, "
      << "SessionID, " << data.SessionID << endl
      << "错误代码, "
      << "ErrorID, " << data.ErrorID << endl
      << "错误信息, "
      << "ErrorMsg, " << data.ErrorMsg << endl
      << "流水号, "
      << "Reference, " << data.Reference << endl
      << "本流水号发布者的机构类型, "
      << "RefrenceIssureType, " << data.RefrenceIssureType << endl
      << "本流水号发布者机构编码, "
      << "RefrenceIssure, " << data.RefrenceIssure << endl
      << "原始返回代码, "
      << "OriginReturnCode, " << data.OriginReturnCode << endl
      << "原始返回码描述, "
      << "OriginDescrInfoForReturnCode, " << data.OriginDescrInfoForReturnCode
      << endl
      << "银行帐号, "
      << "BankAccount, " << data.BankAccount << endl
      << "银行密码, "
      << "BankPassWord, " << data.BankPassWord << endl
      << "投资者帐号, "
      << "AccountID, " << data.AccountID << endl
      << "期货密码, "
      << "Password, " << data.Password << endl
      << "币种代码, "
      << "CurrencyID, " << data.CurrencyID << endl
      << "转帐金额, "
      << "TradeAmount, " << data.TradeAmount << endl
      << "摘要, "
      << "Digest, " << data.Digest << endl;
  return out;
}

/* 日终文件就绪请求 */
ostream &operator<<(ostream &out, CThostFtdcReqDayEndFileReadyField &data) {
  out << "CThostFtdcReqDayEndFileReadyField" << endl
      << "业务功能码, "
      << "TradeCode, " << data.TradeCode << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分支机构代码, "
      << "BankBranchID, " << data.BankBranchID << endl
      << "期商代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "期商分支机构代码, "
      << "BrokerBranchID, " << data.BrokerBranchID << endl
      << "交易日期, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "银行流水号, "
      << "BankSerial, " << data.BankSerial << endl
      << "交易系统日期 , "
      << "TradingDay, " << data.TradingDay << endl
      << "银期平台消息流水号, "
      << "PlateSerial, " << data.PlateSerial << endl
      << "最后分片标志, "
      << "LastFragment, " << data.LastFragment << endl
      << "会话号, "
      << "SessionID, " << data.SessionID << endl
      << "文件业务功能, "
      << "FileBusinessCode, " << data.FileBusinessCode << endl
      << "摘要, "
      << "Digest, " << data.Digest << endl;
  return out;
}

/* 返回结果 */
ostream &operator<<(ostream &out, CThostFtdcReturnResultField &data) {
  out << "CThostFtdcReturnResultField" << endl
      << "返回代码, "
      << "ReturnCode, " << data.ReturnCode << endl
      << "返回码描述, "
      << "DescrInfoForReturnCode, " << data.DescrInfoForReturnCode << endl;
  return out;
}

/* 验证期货资金密码 */
ostream &operator<<(ostream &out, CThostFtdcVerifyFuturePasswordField &data) {
  out << "CThostFtdcVerifyFuturePasswordField" << endl
      << "业务功能码, "
      << "TradeCode, " << data.TradeCode << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分支机构代码, "
      << "BankBranchID, " << data.BankBranchID << endl
      << "期商代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "期商分支机构代码, "
      << "BrokerBranchID, " << data.BrokerBranchID << endl
      << "交易日期, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "银行流水号, "
      << "BankSerial, " << data.BankSerial << endl
      << "交易系统日期 , "
      << "TradingDay, " << data.TradingDay << endl
      << "银期平台消息流水号, "
      << "PlateSerial, " << data.PlateSerial << endl
      << "最后分片标志, "
      << "LastFragment, " << data.LastFragment << endl
      << "会话号, "
      << "SessionID, " << data.SessionID << endl
      << "投资者帐号, "
      << "AccountID, " << data.AccountID << endl
      << "期货密码, "
      << "Password, " << data.Password << endl
      << "银行帐号, "
      << "BankAccount, " << data.BankAccount << endl
      << "银行密码, "
      << "BankPassWord, " << data.BankPassWord << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "交易ID, "
      << "TID, " << data.TID << endl;
  return out;
}

/* 验证客户信息 */
ostream &operator<<(ostream &out, CThostFtdcVerifyCustInfoField &data) {
  out << "CThostFtdcVerifyCustInfoField" << endl
      << "客户姓名, "
      << "CustomerName, " << data.CustomerName << endl
      << "证件类型, "
      << "IdCardType, " << data.IdCardType << endl
      << "证件号码, "
      << "IdentifiedCardNo, " << data.IdentifiedCardNo << endl
      << "客户类型, "
      << "CustType, " << data.CustType << endl;
  return out;
}

/* 验证期货资金密码和客户信息 */
ostream &operator<<(ostream &out,
                    CThostFtdcVerifyFuturePasswordAndCustInfoField &data) {
  out << "CThostFtdcVerifyFuturePasswordAndCustInfoField" << endl
      << "客户姓名, "
      << "CustomerName, " << data.CustomerName << endl
      << "证件类型, "
      << "IdCardType, " << data.IdCardType << endl
      << "证件号码, "
      << "IdentifiedCardNo, " << data.IdentifiedCardNo << endl
      << "客户类型, "
      << "CustType, " << data.CustType << endl
      << "投资者帐号, "
      << "AccountID, " << data.AccountID << endl
      << "期货密码, "
      << "Password, " << data.Password << endl;
  return out;
}

/* 验证期货资金密码和客户信息 */
ostream &operator<<(ostream &out, CThostFtdcDepositResultInformField &data) {
  out << "CThostFtdcDepositResultInformField" << endl
      << "出入金流水号，该流水号为银期报盘返回的流水号, "
      << "DepositSeqNo, " << data.DepositSeqNo << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "入金金额, "
      << "Deposit, " << data.Deposit << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "返回代码, "
      << "ReturnCode, " << data.ReturnCode << endl
      << "返回码描述, "
      << "DescrInfoForReturnCode, " << data.DescrInfoForReturnCode << endl;
  return out;
}

/* 交易核心向银期报盘发出密钥同步请求 */
ostream &operator<<(ostream &out, CThostFtdcReqSyncKeyField &data) {
  out << "CThostFtdcReqSyncKeyField" << endl
      << "业务功能码, "
      << "TradeCode, " << data.TradeCode << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分支机构代码, "
      << "BankBranchID, " << data.BankBranchID << endl
      << "期商代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "期商分支机构代码, "
      << "BrokerBranchID, " << data.BrokerBranchID << endl
      << "交易日期, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "银行流水号, "
      << "BankSerial, " << data.BankSerial << endl
      << "交易系统日期 , "
      << "TradingDay, " << data.TradingDay << endl
      << "银期平台消息流水号, "
      << "PlateSerial, " << data.PlateSerial << endl
      << "最后分片标志, "
      << "LastFragment, " << data.LastFragment << endl
      << "会话号, "
      << "SessionID, " << data.SessionID << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "用户标识, "
      << "UserID, " << data.UserID << endl
      << "交易核心给银期报盘的消息, "
      << "Message, " << data.Message << endl
      << "渠道标志, "
      << "DeviceID, " << data.DeviceID << endl
      << "期货公司银行编码, "
      << "BrokerIDByBank, " << data.BrokerIDByBank << endl
      << "交易柜员, "
      << "OperNo, " << data.OperNo << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "交易ID, "
      << "TID, " << data.TID << endl;
  return out;
}

/* 交易核心向银期报盘发出密钥同步响应 */
ostream &operator<<(ostream &out, CThostFtdcRspSyncKeyField &data) {
  out << "CThostFtdcRspSyncKeyField" << endl
      << "业务功能码, "
      << "TradeCode, " << data.TradeCode << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分支机构代码, "
      << "BankBranchID, " << data.BankBranchID << endl
      << "期商代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "期商分支机构代码, "
      << "BrokerBranchID, " << data.BrokerBranchID << endl
      << "交易日期, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "银行流水号, "
      << "BankSerial, " << data.BankSerial << endl
      << "交易系统日期 , "
      << "TradingDay, " << data.TradingDay << endl
      << "银期平台消息流水号, "
      << "PlateSerial, " << data.PlateSerial << endl
      << "最后分片标志, "
      << "LastFragment, " << data.LastFragment << endl
      << "会话号, "
      << "SessionID, " << data.SessionID << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "用户标识, "
      << "UserID, " << data.UserID << endl
      << "交易核心给银期报盘的消息, "
      << "Message, " << data.Message << endl
      << "渠道标志, "
      << "DeviceID, " << data.DeviceID << endl
      << "期货公司银行编码, "
      << "BrokerIDByBank, " << data.BrokerIDByBank << endl
      << "交易柜员, "
      << "OperNo, " << data.OperNo << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "交易ID, "
      << "TID, " << data.TID << endl
      << "错误代码, "
      << "ErrorID, " << data.ErrorID << endl
      << "错误信息, "
      << "ErrorMsg, " << data.ErrorMsg << endl;
  return out;
}

/* 查询账户信息通知 */
ostream &operator<<(ostream &out, CThostFtdcNotifyQueryAccountField &data) {
  out << "CThostFtdcNotifyQueryAccountField" << endl
      << "业务功能码, "
      << "TradeCode, " << data.TradeCode << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分支机构代码, "
      << "BankBranchID, " << data.BankBranchID << endl
      << "期商代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "期商分支机构代码, "
      << "BrokerBranchID, " << data.BrokerBranchID << endl
      << "交易日期, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "银行流水号, "
      << "BankSerial, " << data.BankSerial << endl
      << "交易系统日期 , "
      << "TradingDay, " << data.TradingDay << endl
      << "银期平台消息流水号, "
      << "PlateSerial, " << data.PlateSerial << endl
      << "最后分片标志, "
      << "LastFragment, " << data.LastFragment << endl
      << "会话号, "
      << "SessionID, " << data.SessionID << endl
      << "客户姓名, "
      << "CustomerName, " << data.CustomerName << endl
      << "证件类型, "
      << "IdCardType, " << data.IdCardType << endl
      << "证件号码, "
      << "IdentifiedCardNo, " << data.IdentifiedCardNo << endl
      << "客户类型, "
      << "CustType, " << data.CustType << endl
      << "银行帐号, "
      << "BankAccount, " << data.BankAccount << endl
      << "银行密码, "
      << "BankPassWord, " << data.BankPassWord << endl
      << "投资者帐号, "
      << "AccountID, " << data.AccountID << endl
      << "期货密码, "
      << "Password, " << data.Password << endl
      << "期货公司流水号, "
      << "FutureSerial, " << data.FutureSerial << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "用户标识, "
      << "UserID, " << data.UserID << endl
      << "验证客户证件号码标志, "
      << "VerifyCertNoFlag, " << data.VerifyCertNoFlag << endl
      << "币种代码, "
      << "CurrencyID, " << data.CurrencyID << endl
      << "摘要, "
      << "Digest, " << data.Digest << endl
      << "银行帐号类型, "
      << "BankAccType, " << data.BankAccType << endl
      << "渠道标志, "
      << "DeviceID, " << data.DeviceID << endl
      << "期货单位帐号类型, "
      << "BankSecuAccType, " << data.BankSecuAccType << endl
      << "期货公司银行编码, "
      << "BrokerIDByBank, " << data.BrokerIDByBank << endl
      << "期货单位帐号, "
      << "BankSecuAcc, " << data.BankSecuAcc << endl
      << "银行密码标志, "
      << "BankPwdFlag, " << data.BankPwdFlag << endl
      << "期货资金密码核对标志, "
      << "SecuPwdFlag, " << data.SecuPwdFlag << endl
      << "交易柜员, "
      << "OperNo, " << data.OperNo << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "交易ID, "
      << "TID, " << data.TID << endl
      << "银行可用金额, "
      << "BankUseAmount, " << data.BankUseAmount << endl
      << "银行可取金额, "
      << "BankFetchAmount, " << data.BankFetchAmount << endl
      << "错误代码, "
      << "ErrorID, " << data.ErrorID << endl
      << "错误信息, "
      << "ErrorMsg, " << data.ErrorMsg << endl;
  return out;
}

/* 银期转账交易流水表 */
ostream &operator<<(ostream &out, CThostFtdcTransferSerialField &data) {
  out << "CThostFtdcTransferSerialField" << endl
      << "平台流水号, "
      << "PlateSerial, " << data.PlateSerial << endl
      << "交易发起方日期, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易日期, "
      << "TradingDay, " << data.TradingDay << endl
      << "交易时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "交易代码, "
      << "TradeCode, " << data.TradeCode << endl
      << "会话编号, "
      << "SessionID, " << data.SessionID << endl
      << "银行编码, "
      << "BankID, " << data.BankID << endl
      << "银行分支机构编码, "
      << "BankBranchID, " << data.BankBranchID << endl
      << "银行帐号类型, "
      << "BankAccType, " << data.BankAccType << endl
      << "银行帐号, "
      << "BankAccount, " << data.BankAccount << endl
      << "银行流水号, "
      << "BankSerial, " << data.BankSerial << endl
      << "期货公司编码, "
      << "BrokerID, " << data.BrokerID << endl
      << "期商分支机构代码, "
      << "BrokerBranchID, " << data.BrokerBranchID << endl
      << "期货公司帐号类型, "
      << "FutureAccType, " << data.FutureAccType << endl
      << "投资者帐号, "
      << "AccountID, " << data.AccountID << endl
      << "投资者代码, "
      << "InvestorID, " << data.InvestorID << endl
      << "期货公司流水号, "
      << "FutureSerial, " << data.FutureSerial << endl
      << "证件类型, "
      << "IdCardType, " << data.IdCardType << endl
      << "证件号码, "
      << "IdentifiedCardNo, " << data.IdentifiedCardNo << endl
      << "币种代码, "
      << "CurrencyID, " << data.CurrencyID << endl
      << "交易金额, "
      << "TradeAmount, " << data.TradeAmount << endl
      << "应收客户费用, "
      << "CustFee, " << data.CustFee << endl
      << "应收期货公司费用, "
      << "BrokerFee, " << data.BrokerFee << endl
      << "有效标志, "
      << "AvailabilityFlag, " << data.AvailabilityFlag << endl
      << "操作员, "
      << "OperatorCode, " << data.OperatorCode << endl
      << "新银行帐号, "
      << "BankNewAccount, " << data.BankNewAccount << endl
      << "错误代码, "
      << "ErrorID, " << data.ErrorID << endl
      << "错误信息, "
      << "ErrorMsg, " << data.ErrorMsg << endl;
  return out;
}

/* 请求查询转帐流水 */
ostream &operator<<(ostream &out, CThostFtdcQryTransferSerialField &data) {
  out << "CThostFtdcQryTransferSerialField" << endl
      << "经纪公司代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "投资者帐号, "
      << "AccountID, " << data.AccountID << endl
      << "银行编码, "
      << "BankID, " << data.BankID << endl;
  return out;
}

/* 期商签到通知 */
ostream &operator<<(ostream &out, CThostFtdcNotifyFutureSignInField &data) {
  out << "CThostFtdcNotifyFutureSignInField" << endl
      << "业务功能码, "
      << "TradeCode, " << data.TradeCode << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分支机构代码, "
      << "BankBranchID, " << data.BankBranchID << endl
      << "期商代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "期商分支机构代码, "
      << "BrokerBranchID, " << data.BrokerBranchID << endl
      << "交易日期, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "银行流水号, "
      << "BankSerial, " << data.BankSerial << endl
      << "交易系统日期 , "
      << "TradingDay, " << data.TradingDay << endl
      << "银期平台消息流水号, "
      << "PlateSerial, " << data.PlateSerial << endl
      << "最后分片标志, "
      << "LastFragment, " << data.LastFragment << endl
      << "会话号, "
      << "SessionID, " << data.SessionID << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "用户标识, "
      << "UserID, " << data.UserID << endl
      << "摘要, "
      << "Digest, " << data.Digest << endl
      << "币种代码, "
      << "CurrencyID, " << data.CurrencyID << endl
      << "渠道标志, "
      << "DeviceID, " << data.DeviceID << endl
      << "期货公司银行编码, "
      << "BrokerIDByBank, " << data.BrokerIDByBank << endl
      << "交易柜员, "
      << "OperNo, " << data.OperNo << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "交易ID, "
      << "TID, " << data.TID << endl
      << "错误代码, "
      << "ErrorID, " << data.ErrorID << endl
      << "错误信息, "
      << "ErrorMsg, " << data.ErrorMsg << endl
      << "PIN密钥, "
      << "PinKey, " << data.PinKey << endl
      << "MAC密钥, "
      << "MacKey, " << data.MacKey << endl;
  return out;
}

/* 期商签退通知 */
ostream &operator<<(ostream &out, CThostFtdcNotifyFutureSignOutField &data) {
  out << "CThostFtdcNotifyFutureSignOutField" << endl
      << "业务功能码, "
      << "TradeCode, " << data.TradeCode << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分支机构代码, "
      << "BankBranchID, " << data.BankBranchID << endl
      << "期商代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "期商分支机构代码, "
      << "BrokerBranchID, " << data.BrokerBranchID << endl
      << "交易日期, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "银行流水号, "
      << "BankSerial, " << data.BankSerial << endl
      << "交易系统日期 , "
      << "TradingDay, " << data.TradingDay << endl
      << "银期平台消息流水号, "
      << "PlateSerial, " << data.PlateSerial << endl
      << "最后分片标志, "
      << "LastFragment, " << data.LastFragment << endl
      << "会话号, "
      << "SessionID, " << data.SessionID << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "用户标识, "
      << "UserID, " << data.UserID << endl
      << "摘要, "
      << "Digest, " << data.Digest << endl
      << "币种代码, "
      << "CurrencyID, " << data.CurrencyID << endl
      << "渠道标志, "
      << "DeviceID, " << data.DeviceID << endl
      << "期货公司银行编码, "
      << "BrokerIDByBank, " << data.BrokerIDByBank << endl
      << "交易柜员, "
      << "OperNo, " << data.OperNo << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "交易ID, "
      << "TID, " << data.TID << endl
      << "错误代码, "
      << "ErrorID, " << data.ErrorID << endl
      << "错误信息, "
      << "ErrorMsg, " << data.ErrorMsg << endl;
  return out;
}

/* 交易核心向银期报盘发出密钥同步处理结果的通知 */
ostream &operator<<(ostream &out, CThostFtdcNotifySyncKeyField &data) {
  out << "CThostFtdcNotifySyncKeyField" << endl
      << "业务功能码, "
      << "TradeCode, " << data.TradeCode << endl
      << "银行代码, "
      << "BankID, " << data.BankID << endl
      << "银行分支机构代码, "
      << "BankBranchID, " << data.BankBranchID << endl
      << "期商代码, "
      << "BrokerID, " << data.BrokerID << endl
      << "期商分支机构代码, "
      << "BrokerBranchID, " << data.BrokerBranchID << endl
      << "交易日期, "
      << "TradeDate, " << data.TradeDate << endl
      << "交易时间, "
      << "TradeTime, " << data.TradeTime << endl
      << "银行流水号, "
      << "BankSerial, " << data.BankSerial << endl
      << "交易系统日期 , "
      << "TradingDay, " << data.TradingDay << endl
      << "银期平台消息流水号, "
      << "PlateSerial, " << data.PlateSerial << endl
      << "最后分片标志, "
      << "LastFragment, " << data.LastFragment << endl
      << "会话号, "
      << "SessionID, " << data.SessionID << endl
      << "安装编号, "
      << "InstallID, " << data.InstallID << endl
      << "用户标识, "
      << "UserID, " << data.UserID << endl
      << "交易核心给银期报盘的消息, "
      << "Message, " << data.Message << endl
      << "渠道标志, "
      << "DeviceID, " << data.DeviceID << endl
      << "期货公司银行编码, "
      << "BrokerIDByBank, " << data.BrokerIDByBank << endl
      << "交易柜员, "
      << "OperNo, " << data.OperNo << endl
      << "请求编号, "
      << "RequestID, " << data.RequestID << endl
      << "交易ID, "
      << "TID, " << data.TID << endl
      << "错误代码, "
      << "ErrorID, " << data.ErrorID << endl
      << "错误信息, "
      << "ErrorMsg, " << data.ErrorMsg << endl;
  return out;
}

/* 发给做市商的询价请求 */
ostream &operator<<(ostream &out, CThostFtdcForQuoteRspField &data) {
  out << "发给做市商的询价请求 CThostFtdcForQuoteRspField" << endl
      << "交易日, " << data.TradingDay << endl
      << "合约代码, " << data.InstrumentID << endl
      << "询价编号, " << data.ForQuoteSysID << endl
      << "询价时间, " << data.ForQuoteTime << endl
      << "业务日期, " << data.ActionDay << endl
      << "交易所代码, " << data.ExchangeID << endl;

  return out;
}

/* 客户端认证响应 */
ostream &operator<<(ostream &out, CThostFtdcRspAuthenticateField &data) {
  out << "客户端认证响应 CThostFtdcRspAuthenticateField, " << endl
      << "经纪公司代码, " << data.BrokerID << endl
      << "用户代码, " << data.UserID << endl
      << "用户端产品信息, " << data.UserProductInfo << endl;
  return out;
}
