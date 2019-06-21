```c++
class QRCodeDetector
{
public:
    
    //默认构造函数，初始化时会给p赋值new Impl
    //p->epsX = 0.2, P->epsY = 0.1
    //也就是说 QRCodeDetector qrcode; 创建一个对象时就会有
    //qrcode.p->epsX = 0.2, qrcode.P->epsY = 0.1
    
    QRCodeDetector() : p(new Impl) {}

    //两个设置eps的成员函数

    void setEpsX(double epsX) { p->epsX = epsX; }
    void setEpsY(double epsY) { p->epsY = epsY; }

    //成员函数 detect(img, points)
    //接收一个 Mat&, 一个 vector<Point>&(返回的四个角点的坐标)

    bool detect(cv::InputArray img, cv::OutputArray points)
    {
        //从参数 img 中读入一个 Mat inarr
        //如果为空，或有一个维度小于20，返回 false
        //如果 Mat 的通道数为3或4，转换为灰度图
        //创建 QRDetect 对象 qrdet
        //调用 qrdet.init(inarr, p->epsX, p->epsY),设置 qrdet.coeff_expansion, qrdet.barcode, qrdet.bin_barcode(二值化图像), qrdet.eps_vertical, qrdet.eps_horizontal
        //调用 qrdet.localization(),fix过的3点放在localization_points
        //调用 qrdet.computeTransformationPoints()
        //调用 qrdet.getTransformationPoints()
    }

protected:

    //数据成员类 Impl类
    //默认初始化后: epsX = 0.2; epsY = 0.1; 
    // QRCodeDetector 的默认构造函数时初始化 *p = new Impl
    struct Impl
    {
    public:
        Impl() { epsX = 0.2; epsY = 0.1; } //默认构造函数
        ~Impl() {} //析构函数

        double epsX, epsY;
    };

    //数据成员 模板指针p???
    //会在 QRCodeDetector 的默认构造函数时初始化为 new Impl
    cv::Ptr<Impl> p;
};
```

QRDetect 类
```c++
class QRDetect
{
public:

    //QRCodeDetector.detect() 调用 qrdet.init()
    //设置 bin_barcode ...

    void init(const Mat& src, double eps_vertical_ = 0.2, double eps_horizontal_ = 0.1)
    {
        //如果有一边小于512，放大尺寸到512，并设置 qrdet.coeff_expansion
        //把改变尺寸的 src 给 qrdet.barcode

        resize(src, barcode, new_size, 0, 0, INTER_LINEAR); 
        //(源, 目的, 新尺寸, 比例因子x, 比例因子y, 插值方法)

        //如果没有小于512，设置 qrdet.coeff_expansion = 1.0, qrdet.barcode = src
        //设置 eps_vertical = 0.2，eps_horizontal = 0.1
        //调用 adaptiveThreshold()自适应阈值二值化，存放到 qrdet.bin_barcode
    }

    //QRCodeDetector.detect() 调用 qrdet.localization(),返回false则return
    //fix 过的点放在localization_points
    
    bool localization()
    {
        //调用 searchHorizontalLines(),得到水平方向的符合比例的位置
        //根据水平的结果调用 separateVerticalLines()，得到一系列中心点坐标
        //kmeans()根据这些点返回聚类的三个中心点到qrdet.localization_points
        //调用 fixationPoints()，对点的位置再做验证，余弦，轮廓面积，最后保证中心点是第一个，按逆时针方向
        //如果放缩系数coeff_expansion大于1，缩小到原尺寸，同时调整点坐标
        //若有两个点的距离小于10，return false
    }

    //
    bool computeTransformationPoints()
    {

    }

    //返回所有在水平方向符合比例的位置

    vector<Vec3d> searchHorizontalLines()
    {
        //每一行搜索第一个黑色像素位置，然后记下之后每一次黑白互换的位置
        //在这些位置中从前向后每次计算5段黑白段的长度，算出每一段的比例与1:1:3:1:1的误差，若误差小于阈值，记下一个结果点
        //vec3d line[0] 第一个黑色像素的位置
        //vec3d line[1] 当前的第几行
        //vec3d line[2] 从前到后匹配的图案的总长度
    }

    //返回一系列在识别块中心附近的点的坐标

    vector<Point2f> separateVerticalLines(const vector<Vec3d> &list_lines)
    {
        //根据水平搜索的结果，先得到这条线的中心点，该中心点应该在识别块的中心或附近，同样的方法在该点向上或向下搜索三段跳动的长度，计算出比例的误差，最终返回中心点坐标
        //vector<Point2f> point2f_result = {(x1, y1), (x2, y2), (x3, y3)...}
    }
};
```