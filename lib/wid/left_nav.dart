import 'package:astral/k/app_s/aps.dart';
import 'package:astral/k/navigtion.dart';
import 'package:flutter/material.dart';

class LeftNav extends StatelessWidget {
  final List<NavigationItem> items;
  final ColorScheme colorScheme;

  const LeftNav({super.key, required this.items, required this.colorScheme});

  @override
  Widget build(BuildContext context) {
    // 构建单个导航项组件
    Widget buildNavItem(
      IconData icon,
      String label,
      int index,
      ColorScheme colorScheme,
      dynamic item,
    ) {
      final isSelected = Aps().selectedIndex.watch(context) == index;
      return MouseRegion(
        onEnter: (_) => Aps().hoveredIndex.set(index), // 设置鼠标悬停索引
        onExit: (_) => Aps().hoveredIndex.set(null), // 鼠标移出重置
        child: Container(
          height: 64,
          margin: const EdgeInsets.symmetric(vertical: 4, horizontal: 8), // 外边距
          child: InkWell(
            borderRadius: BorderRadius.circular(12), // 点击水波边界
            onTap: () {
              if (!isSelected) {
                Aps().selectedIndex.set(index); // 更新选中项
              }
            },
            child: Center(
              child: AnimatedDefaultTextStyle(
                duration: const Duration(milliseconds: 200), // 文字样式动画
                style: TextStyle(
                  fontWeight: FontWeight.bold,
                  color: isSelected
                      ? colorScheme.primary
                      : colorScheme.onSurfaceVariant,
                ),
                child: Column(
                  mainAxisSize: MainAxisSize.min,
                  children: [
                    AnimatedSwitcher(
                      duration: const Duration(milliseconds: 200), // 图标切换动画
                      child: Icon(
                        isSelected ? item.activeIcon : item.icon,
                        color: isSelected
                            ? colorScheme.primary
                            : colorScheme.onSurfaceVariant,
                        size: 24,
                        key: ValueKey(isSelected), // 使用状态作为 key
                      ),
                    ),
                    const SizedBox(height: 4), // 图标与文字间距
                    Text(item.label),
                  ],
                ),
              ),
            ),
          ),
        ),
      );
    }

    return Container(
      width: 80, // 导航栏宽度
      decoration: BoxDecoration(
        color: colorScheme.surface,
        border: Border(
          right: BorderSide(color: colorScheme.outline, width: 1), // 右边框
        ),
      ),
      child: Padding(
        padding: const EdgeInsets.only(top: 14), // 顶部留白
        child: Stack(
          children: [
            // 滑动选中指示器动画
            Positioned(
              left: 8,
              right: 8,
              child: TweenAnimationBuilder<double>(
                duration: const Duration(milliseconds: 300),
                curve: Curves.easeOutCubic,
                tween: Tween<double>(
                  begin: 4.0 + (Aps().selectedIndex.watch(context) * 72.0),
                  end: 4.0 + (Aps().selectedIndex.watch(context) * 72.0),
                ),
                builder: (context, value, child) {
                  return Positioned(
                    top: value,
                    height: 64,
                    child: Container(
                      decoration: BoxDecoration(
                        color: colorScheme.primaryContainer,
                        borderRadius: BorderRadius.circular(12), // ✅ 正确写法
                      ),
                    ),
                  );
                },
              ),
            ),

            // 鼠标悬停高亮指示器
            if (Aps().hoveredIndex.watch(context) != null &&
                Aps().hoveredIndex.watch(context) !=
                    Aps().selectedIndex.watch(context))
              Positioned(
                top: 4.0 + (Aps().hoveredIndex.watch(context)! * 72.0),
                left: 8,
                right: 8,
                height: 64,
                child: Container(
                  decoration: BoxDecoration(
                    color: colorScheme.surfaceContainerHighest.withOpacity(0.5),
                    borderRadius: BorderRadius.circular(12), // ✅ 修正位置
                  ),
                ),
              ),

            // 导航项列表
            Column(
              children: [
                Expanded(
                  child: ListView.builder(
                    itemCount: items.length,
                    itemBuilder: (context, index) {
                      final item = items[index];
                      return buildNavItem(
                        item.icon,
                        item.label,
                        index,
                        colorScheme,
                        item,
                      );
                    },
                  ),
                ),
              ],
            ),
          ],
        ),
      ),
    );
  }
}
